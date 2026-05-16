#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Math/GJK.hpp"
#include "Woden/Math/Sphere.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace Physics
{
// Collision shape
bool CollisionShape::isCompound() const
{
    return false;
}

bool CollisionShape::isConvex() const
{
    return false;
}

std::optional<ShapeRayCastingResult> CollisionShape::rayCast(const Math::Ray3D &ray)
{
    (void)ray;
    return std::nullopt;
}

Math::Matrix3x3 CollisionShape::computeInertiaTensorWithMass(Math::Scalar mass)
{
    (void)mass;
    return Math::Matrix3x3::Zeros();
}

SceneGraph::SceneNodePtr CollisionShape::constructVisualizationSceneNode()
{
    return nullptr;
}

// Convex collision shape
bool ConvexCollisionShape::isConvex() const
{
    return true;
}

std::vector<ContactPoint> ConvexCollisionShape::detectAndComputeCollisionContactPoints(const Math::RigidTransform &myTransform, const CollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis)
{
    auto contactPoints = otherShape->detectAndComputeConvexCollisionContactPoints(otherShapeTransform, std::static_pointer_cast<ConvexCollisionShape> (shared_from_this()), myTransform, -initialSeparatingAxis);
    for(auto &contact : contactPoints)
        contact.flip();
    return contactPoints;
}

std::vector<ContactPoint> ConvexCollisionShape::detectAndComputeCompoundCollisionContactPoints(const Math::RigidTransform &myTransform, const CompoundCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis)
{
    auto contactPoints = otherShape->detectAndComputeConvexCollisionContactPoints(otherShapeTransform, std::static_pointer_cast<ConvexCollisionShape> (shared_from_this()), myTransform, -initialSeparatingAxis);
    for(auto &contact : contactPoints)
        contact.flip();
    return contactPoints;
}

std::vector<ContactPoint> ConvexCollisionShape::detectAndComputeConvexCollisionContactPoints(const Math::RigidTransform &myTransform, const ConvexCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis)
{
    return detectAndComputeConvexCollisionContactPointsForShape(myTransform, Math::RigidTransform(), otherShape, otherShapeTransform, initialSeparatingAxis);
}

std::vector<ContactPoint> ConvexCollisionShape::detectAndComputeConvexCollisionContactPointsForShape(const Math::RigidTransform &myTransform, const Math::RigidTransform &myShapeTransform, const ConvexCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis)
{
    const Math::Scalar ShallowPenetrationThreshold = 1.0e-5f;

    auto myChildTransform = myTransform.transformTransform(myShapeTransform);

    std::vector<ContactPoint> result;
    auto &&firstSupportFunction = [&](const Math::Vector3 &D) {
        return myChildTransform.transformPosition(localSupportInDirection(myChildTransform.inverseTransformNormalVector(D)));
    };
    auto &&secondSupportFunction = [&](const Math::Vector3 &D) {
        return otherShapeTransform.transformPosition(otherShape->localSupportInDirection(otherShapeTransform.inverseTransformNormalVector(D)));
    };

    auto simplex = computeGJKSimplex(firstSupportFunction, secondSupportFunction, initialSeparatingAxis);

    auto closestPointToOrigin = simplex.getClosestPointToOrigin();
    Math::Scalar totalMargin = margin + otherShape->margin;
    auto shapeDistance = closestPointToOrigin.length();
    if(shapeDistance > totalMargin)
        return result;

    if(shapeDistance > ShallowPenetrationThreshold)
    {
        ContactPoint contactPoint;
        contactPoint.normal = simplex.getClosestPointToOrigin().normalized();
        contactPoint.requiredSeparation = totalMargin;
        contactPoint.firstPoint = simplex.getClosestPointToOriginInFirst();
        contactPoint.secondPoint = simplex.getClosestPointToOriginInSecond();
        contactPoint.computeLocalVersionWithTransforms(myTransform, otherShapeTransform);
        contactPoint.computeWorldContactPointAndDistances();
        result.push_back(contactPoint);
        //printf("Shallow penetration distance: %f| %f %f %f\n", shapeDistance, contactPoint.normal.x, contactPoint.normal.y, contactPoint.normal.z);
    }
    else
    {
        auto optDeepContact = samplePenetrationDistanceAndNormalWithMargin(firstSupportFunction, secondSupportFunction, totalMargin, initialSeparatingAxis);
        if(!optDeepContact.has_value())
            return result;

        auto deepContactSample = optDeepContact.value();
        ContactPoint contactPoint;
        contactPoint.normal = deepContactSample.normal;
        contactPoint.requiredSeparation = totalMargin;
        contactPoint.firstPoint = deepContactSample.firstPoint;
        contactPoint.secondPoint = deepContactSample.secondPoint;
        contactPoint.computeLocalVersionWithTransforms(myTransform, otherShapeTransform);
        contactPoint.computeWorldContactPointAndDistances();
        //printf("Deep penetration %f: %f %f %f\n", deepContactSample.distance, deepContactSample.normal.x, deepContactSample.normal.y, deepContactSample.normal.z);
        result.push_back(contactPoint);
    }

    return result;
}

// Sphere collision shape
Math::Matrix3x3 SphereCollisionShape::computeInertiaTensorWithMass(Math::Scalar mass)
{
    return Math::Matrix3x3(radius * radius * 2 * 5 * mass);
}

Math::Vector3 SphereCollisionShape::localSupportInDirection(const Math::Vector3 &D)
{
    return D.normalized()*Math::Vector3(radius);
}

std::optional<ShapeRayCastingResult> SphereCollisionShape::rayCast(const Math::Ray3D &ray)
{
    auto sphere = Math::Sphere(Math::Vector3(0), radius);
    auto result = sphere.intersectionsWithRay(ray);
    if(result.isEmpty())
        return std::nullopt;

    ShapeRayCastingResult shapeResult;
    shapeResult.distance = result.intersectionPoints[0];
    shapeResult.point = ray.pointAtDistance(shapeResult.distance);
    shapeResult.normal = sphere.computeNormalForPoint(shapeResult.point);
    return shapeResult;
}

// Box collision shape
Math::Matrix3x3 BoxCollisionShape::computeInertiaTensorWithMass(Math::Scalar mass)
{
    auto extent = halfExtent* Math::Vector3(2.0);
    auto extent2 = extent*extent;
    return Math::Matrix3x3(
        (extent2.y + extent2.z)/12*mass, 0, 0,
        0, (extent2.x + extent2.z)/12*mass, 0,
        0, 0, (extent2.x + extent2.y)/12*mass
    );
}

Math::Vector3 BoxCollisionShape::localSupportInDirection(const Math::Vector3 &D)
{
    return localBoundingBox.support(D);
}

std::optional<ShapeRayCastingResult> BoxCollisionShape::rayCast(const Math::Ray3D &ray)
{
    auto box = Math::AABox::WithHalfExtent(halfExtent);

    auto result = box.intersectionsWithRay(ray);
    if(result.isEmpty())
        return std::nullopt;

    ShapeRayCastingResult shapeResult;
    shapeResult.distance = result.intersectionPoints[0];
    shapeResult.point = ray.pointAtDistance(shapeResult.distance);
    shapeResult.normal = box.computeNormalForPoint(shapeResult.point);
    return shapeResult;
}

SceneGraph::SceneNodePtr BoxCollisionShape::constructVisualizationSceneNode()
{
    return Woden::Rendering::MeshBuilder()
            .addCubeWithBox(localBoundingBox)
            .generateTexcoordsWithFacePlanarTransformWithScale(Math::Vector2(1, 1))
            .finishMesh()
            ->asSceneNode();
}

// Convex hull collision shape
Math::Matrix3x3 ConvexHullCollisionShape::computeInertiaTensorWithMass(Math::Scalar mass)
{
    // For now use the box
    auto extent = localBoundingBox.halfExtent()* Math::Vector3(2.0);
    auto extent2 = extent*extent;
    return Math::Matrix3x3(
        (extent2.y + extent2.z)/12*mass, 0, 0,
        0, (extent2.x + extent2.z)/12*mass, 0,
        0, 0, (extent2.x + extent2.y)/12*mass
    );
}

Math::Vector3 ConvexHullCollisionShape::localSupportInDirection(const Math::Vector3 &D)
{
    auto bestDistance = -INFINITY;
    auto bestFound = Math::Vector3();
    for(const auto &corner : corners)
    {
        auto distance = corner.dot(D);
        if(distance > bestDistance)
        {
            bestDistance = distance;
            bestFound = corner;
        }
    }

    return bestFound;
}

std::optional<ShapeRayCastingResult> ConvexHullCollisionShape::rayCast(const Math::Ray3D &ray)
{
    auto gjkResult = Math::computeGJKRayCasting(ray, [&](const Math::Vector3 &D){
        return this->localSupportInDirection(D);
    });

    if (!gjkResult.has_value())
        return std::nullopt;

    auto result = gjkResult.value();

    ShapeRayCastingResult shapeResult;
    shapeResult.distance = result.first;
    shapeResult.point = ray.pointAtDistance(result.first);
    shapeResult.normal = result.second.normalized();
    return shapeResult;
}

SceneGraph::SceneNodePtr ConvexHullCollisionShape::constructVisualizationSceneNode()
{
    return Woden::Rendering::MeshBuilder()
            .addCubeWithBox(localBoundingBox)
            .generateTexcoordsWithFacePlanarTransformWithScale(Math::Vector2(1, 1))
            .finishMesh()
            ->asSceneNode();
}

// Capsule collision shape
void CapsuleYCollisionShape::computeLocalBoundingBox()
{
    localBoundingBox = Math::AABox::WithHalfExtent(Math::Vector3(radius, halfHeight + radius, radius));
    localBoundingBoxWithMargin = localBoundingBox.expandedBy(margin);
}

SceneGraph::SceneNodePtr CapsuleYCollisionShape::constructVisualizationSceneNode()
{
    return Woden::Rendering::MeshBuilder()
            .addCubeWithHalfExtent(Math::Vector3(radius, halfHeight + radius, radius))
            .generateTexcoordsWithFacePlanarTransformWithScale(Math::Vector2(1, 1))
            .finishMesh()
            ->asSceneNode();
}

Math::Vector3 CapsuleYCollisionShape::localSupportInDirection(const Math::Vector3 &D)
{
    return Math::Vector3(0, Math::sign(D.y)*halfHeight, 0) + (D.normalized() * Math::Vector3(radius));
}

bool CompoundCollisionShape::isCompound() const
{
    return true;
}

SceneGraph::SceneNodePtr CompoundCollisionShape::constructVisualizationSceneNode()
{
    auto sceneNode = std::make_shared<SceneGraph::SceneNode> ();
    for(auto &child : children)
    {
        auto childSceneNode = child->shape->constructVisualizationSceneNode();
        if(!childSceneNode)
            continue;

        childSceneNode->transform = child->transform.asTRSTransform3D();
        sceneNode->addChild(childSceneNode);
    }

    return sceneNode;
}

std::vector<ContactPoint> CompoundCollisionShape::detectAndComputeCollisionContactPoints(const Math::RigidTransform &myTransform, const CollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis)
{
    auto contactPoints = otherShape->detectAndComputeCompoundCollisionContactPoints(otherShapeTransform, std::static_pointer_cast<CompoundCollisionShape> (shared_from_this()), myTransform, -initialSeparatingAxis);
    for(auto &contact : contactPoints)
        contact.flip();
    return contactPoints;
}

std::vector<ContactPoint> CompoundCollisionShape::detectAndComputeConvexCollisionContactPoints(const Math::RigidTransform &myTransform, const ConvexCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis)
{
    // Compound vs Convex
    std::vector<ContactPoint> contactPoints;

    auto otherShapeBox = otherShape->localBoundingBoxWithMargin.transformedWith(otherShapeTransform).inverseTransformedWith(myTransform);
    bvh.leavesIntersectingBoxDo(otherShapeBox, [&](const CompoundCollisionShapeChildPtr &child){
        auto childContactPoints = child->shape->detectAndComputeConvexCollisionContactPointsForShape(myTransform, child->transform, otherShape, otherShapeTransform, initialSeparatingAxis);
        contactPoints.insert(contactPoints.end(), childContactPoints.begin(), childContactPoints.end());
    });

   return contactPoints;
}

std::vector<ContactPoint> CompoundCollisionShape::detectAndComputeCompoundCollisionContactPoints(const Math::RigidTransform &myTransform, const CompoundCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis)
{
    // TODO: Compound vs compound;
    (void)myTransform;
    (void)otherShape;
    (void)otherShapeTransform;
    (void)initialSeparatingAxis;
    return std::vector<ContactPoint>();
}

void CompoundCollisionShape::addChild(const ConvexCollisionShapePtr &shape, const Math::RigidTransform &transform)
{
    auto child = std::make_shared<CompoundCollisionShapeChild> ();
    child->shape = shape;
    child->transform = transform;
    children.push_back(child);
}

void CompoundCollisionShape::addChildWithTranslation(const ConvexCollisionShapePtr &shape, const Math::Vector3 &translation)
{
    Math::RigidTransform transform;
    transform.translation = translation;
    addChild(shape, transform);
}

void CompoundCollisionShape::finishAddingChildren()
{
    localBoundingBox = Math::AABox::Empty();
    for(auto &child : children)
    {
        child->box = child->shape->localBoundingBoxWithMargin.transformedWith(child->transform);
        localBoundingBox.insertBox(child->box);
    }

    localBoundingBoxWithMargin = localBoundingBox.expandedBy(margin);
    buildBVH();
}

void CompoundCollisionShape::buildBVH()
{
    // Bottom up construction.
    std::vector<CompoundShapeBVH::NodePtrType> bvhLeaves;
    bvhLeaves.reserve(children.size());
    for(auto &child : children)
    {
        auto leaf = std::make_shared<CompoundShapeBVH::NodeType> ();
        leaf->isLeaf = true;
        leaf->volume = child->box;
        leaf->payload = child;
        bvhLeaves.push_back(leaf);
    }

    bvh.buildBottomUp(bvhLeaves);
}

} // End of namespace Physics
} // End of namespace Woden