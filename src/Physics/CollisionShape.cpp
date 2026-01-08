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
bool CollisionShape::isConvex() const
{
    return false;
}

std::optional<ShapeRayCastingResult> CollisionShape::rayCast(const Math::Ray3D &ray)
{
    (void)ray;
    return std::nullopt;
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

std::vector<ContactPoint> ConvexCollisionShape::detectAndComputeConvexCollisionContactPoints(const Math::RigidTransform &myTransform, const ConvexCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis)
{
    const Math::Scalar ShallowPenetrationThreshold = 1.0e-5;

    std::vector<ContactPoint> result;
    auto &&firstSupportFunction = [&](const Math::Vector3 &D) {
        return myTransform.transformPosition(localSupportInDirection(myTransform.inverseTransformNormalVector(D)));
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
            .addCubeWithHalfExtent(halfExtent)
            .generateTexcoordsWithFacePlanarTransformWithScale(Math::Vector2(1, 1))
            .finishMesh()
            ->asSceneNode();
}

} // End of namespace Physics
} // End of namespace Woden