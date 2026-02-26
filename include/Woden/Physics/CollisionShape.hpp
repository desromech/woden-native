#ifndef WODEN_PHYSICS_COLLISION_SHAPE
#define WODEN_PHYSICS_COLLISION_SHAPE

#include "Woden/Math/AABox.hpp"
#include "Woden/Math/Ray3D.hpp"
#include "Woden/Math/RigidTransform.hpp"
#include "ContactManifold.hpp"
#include <memory>
#include <optional>

namespace Woden
{
namespace SceneGraph
{
typedef std::shared_ptr<class SceneNode> SceneNodePtr;
}

namespace Physics
{

typedef std::shared_ptr<class CollisionShape> CollisionShapePtr;
typedef std::shared_ptr<class ConvexCollisionShape> ConvexCollisionShapePtr;
typedef std::shared_ptr<class BoxCollisionShape> BoxCollisionShapePtr;
typedef std::shared_ptr<class SphereCollisionShape> SphereCollisionShapePtr;

typedef std::shared_ptr<class CompoundCollisionShape> CompoundCollisionShapePtr;
typedef std::shared_ptr<class CompoundCollisionShapeChild> CompoundCollisionShapeChildPtr;

struct ShapeRayCastingResult
{
    CollisionShapePtr shape;
    float distance = 0;
    Math::Vector3 point;
    Math::Vector3 normal;
};

// Collision shape
class CollisionShape : public std::enable_shared_from_this<CollisionShape>
{
public:
    virtual bool isCompound() const;
    virtual bool isConvex() const;

    virtual std::optional<ShapeRayCastingResult> rayCast(const Math::Ray3D &ray);

    virtual Math::Matrix3x3 computeInertiaTensorWithMass(Math::Scalar mass);
    virtual SceneGraph::SceneNodePtr constructVisualizationSceneNode();

    virtual std::vector<ContactPoint> detectAndComputeCollisionContactPoints(const Math::RigidTransform &myTransform, const CollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) = 0;
    virtual std::vector<ContactPoint> detectAndComputeConvexCollisionContactPoints(const Math::RigidTransform &myTransform, const ConvexCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) = 0;
    virtual std::vector<ContactPoint> detectAndComputeCompoundCollisionContactPoints(const Math::RigidTransform &myTransform, const CompoundCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) = 0;

    Math::Scalar margin = 0.01f;
    Math::AABox localBoundingBox = Math::AABox(Math::Vector3::Zeros(), Math::Vector3::Zeros());
    Math::AABox localBoundingBoxWithMargin = Math::AABox(Math::Vector3::Zeros(), Math::Vector3::Zeros());
};

// Convex collision shape
class ConvexCollisionShape : public CollisionShape
{
public:
    virtual bool isConvex() const override;
    virtual Math::Vector3 localSupportInDirection(const Math::Vector3 &D) = 0;

    virtual std::vector<ContactPoint> detectAndComputeCollisionContactPoints(const Math::RigidTransform &myTransform, const CollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) override;
    virtual std::vector<ContactPoint> detectAndComputeConvexCollisionContactPoints(const Math::RigidTransform &myTransform, const ConvexCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) override;
    virtual std::vector<ContactPoint> detectAndComputeConvexCollisionContactPointsForShape(const Math::RigidTransform &myTransform, const Math::RigidTransform &myShapeTransform, const ConvexCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis);

    virtual std::vector<ContactPoint> detectAndComputeCompoundCollisionContactPoints(const Math::RigidTransform &myTransform, const CompoundCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) override;

};

// Sphere collision shape
class SphereCollisionShape : public ConvexCollisionShape
{
public:
    Math::Scalar getRadius() const
    {
        return radius;
    }

    void setRadius(Math::Scalar newRadius)
    {
        radius = newRadius;
    }

    virtual Math::Matrix3x3 computeInertiaTensorWithMass(Math::Scalar mass) override;
    virtual Math::Vector3 localSupportInDirection(const Math::Vector3 &D) override;

    virtual std::optional<ShapeRayCastingResult> rayCast(const Math::Ray3D &ray) override;

protected:
    Math::Scalar radius = 1;
};

// AAbox collision shape
class BoxCollisionShape : public ConvexCollisionShape
{
public:
    Math::Vector3 getHalfExtent() const
    {
        return halfExtent;
    }

    void setHalfExtent(Math::Vector3 newHalfExtent)
    {
        halfExtent = newHalfExtent;
        localBoundingBox = Math::AABox::WithHalfExtent(newHalfExtent);
        localBoundingBoxWithMargin = localBoundingBox.expandedBy(margin);
    }

    virtual Math::Matrix3x3 computeInertiaTensorWithMass(Math::Scalar mass) override;
    virtual Math::Vector3 localSupportInDirection(const Math::Vector3 &D) override;

    virtual std::optional<ShapeRayCastingResult> rayCast(const Math::Ray3D &ray) override;
    virtual SceneGraph::SceneNodePtr constructVisualizationSceneNode() override;

protected:
    Math::Vector3 halfExtent = Math::Vector3(1, 1, 1);
};

// Abstract capsule shape shape
class AbstractCapsuleCollisionShape : public ConvexCollisionShape
{
public:

    void setHeightAndRadius(Math::Scalar newHeight, Math::Scalar newRadius)
    {
        height = newHeight;
        halfHeight = newHeight*0.5f;
        radius = newRadius;
        computeLocalBoundingBox();
    }

    virtual void computeLocalBoundingBox() = 0;

protected:
    Math::Scalar radius = 1;
    Math::Scalar halfHeight = 0.5f;
    Math::Scalar height = 1;
};

// Capsule y shape
class CapsuleYCollisionShape : public AbstractCapsuleCollisionShape
{
public:
    virtual SceneGraph::SceneNodePtr constructVisualizationSceneNode() override;
    virtual void computeLocalBoundingBox() override;
    virtual Math::Vector3 localSupportInDirection(const Math::Vector3 &D) override;
};

struct CompoundCollisionShapeChild
{
    ConvexCollisionShapePtr shape;
    Math::RigidTransform transform;
};

// Compound collision shape
class CompoundCollisionShape : public CollisionShape
{
public:
    virtual bool isCompound() const override;

    virtual SceneGraph::SceneNodePtr constructVisualizationSceneNode() override;

    virtual std::vector<ContactPoint> detectAndComputeCollisionContactPoints(const Math::RigidTransform &myTransform, const CollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) override;
    virtual std::vector<ContactPoint> detectAndComputeConvexCollisionContactPoints(const Math::RigidTransform &myTransform, const ConvexCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) override;
    virtual std::vector<ContactPoint> detectAndComputeCompoundCollisionContactPoints(const Math::RigidTransform &myTransform, const CompoundCollisionShapePtr &otherShape, const Math::RigidTransform &otherShapeTransform, const Math::Vector3 &initialSeparatingAxis) override;

    void addChild(const ConvexCollisionShapePtr &shape, const Math::RigidTransform &transform);
    void addChildWithTranslation(const ConvexCollisionShapePtr &shape, const Math::Vector3 &translation);
    void finishAddingChildren();

    std::vector<CompoundCollisionShapeChildPtr> children;

};

} // End of namespace Physics
} // End of namespace Woden
#endif //WODEN_PHYSICS_COLLISION_SHAPE
