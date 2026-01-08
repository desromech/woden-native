#ifndef WODEN_PHYSICS_COLLISION_OBJECT_HPP
#define WODEN_PHYSICS_COLLISION_OBJECT_HPP

#include "CollisionMasks.hpp"
#include "Woden/Math/AABox.hpp"
#include "Woden/Math/RigidTransform.hpp"
#include <memory>

namespace Woden
{
namespace SceneGraph
{
typedef std::shared_ptr<class SceneNode> SceneNodePtr;
}

namespace Physics
{
typedef std::shared_ptr<class CollisionShape> CollisionShapePtr;
typedef std::shared_ptr<class CollisionObject> CollisionObjectPtr;
typedef std::weak_ptr<class PhysicsWorld> PhysicsWorldWeakPtr;

/**
 * I am an object that participates in colliosions.
 */
class CollisionObject
{
public:

    const Math::Vector3 & getPosition() const
    {
        return transform.translation;
    }

    void setPosition(const Math::Vector3 &newPosition)
    {
        transform.translation = newPosition;
        transformChanged();
    }

    const Math::Quaternion & getOrientation() const
    {
        return transform.rotation;
    }

    void setOrientation(const Math::Quaternion &newOrientation)
    {
        transform.rotation = newOrientation;
        transformChanged();
    }

    void setPositionAndOrientation(const Math::Vector3 &newPosition, const Math::Quaternion &newOrientation)
    {
        transform.translation = newPosition;
        transform.rotation = newOrientation;
        transformChanged();
    }

    const Math::RigidTransform &getTransform() const
    {
        return transform;
    }

    void setTransform(const Math::RigidTransform &newTransform)
    {
        transform = newTransform;
        transformChanged();
    }

    Math::AABox getWorldBoundingBoxWithMargin() const;

    virtual void transformChanged();

    virtual void resetNetForces();
    virtual void integrateMovement(Math::Scalar deltaTime);

    virtual bool needsCollisionDetection();

    SceneGraph::SceneNodePtr constructVisualizationSceneNode();

    virtual Math::Scalar getMass() const;
    virtual Math::Scalar getInverseMass() const;
    virtual void applyMovementAtRelativePoint(Math::Scalar movement, const Math::Vector3 &relativePoint, const Math::Vector3 &normalDirection);
    virtual void applyImpulse(Math::Vector3 impulse);

    PhysicsWorldWeakPtr owner;
    CollisionShapePtr shape;
    
    Math::Vector3 linearVelocity = Math::Vector3(0);
    Math::Vector3 angularVelocity = Math::Vector3(0);

    Math::Scalar restitutionCoefficient = 0.2f;
	Math::Scalar dynamicFrictionCoefficient = 0.5f;
	Math::Scalar staticFrictionCoefficient = 0.6f;

    CollisionMasks collisionGroup = CollisionMasks::DefaultCollisionGroup;
    CollisionMasks collisionResponseGroupMask = CollisionMasks::DefaultCollisionResponseGroup;

protected:
    Math::RigidTransform transform;

};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_COLLISION_OBJECT_HPP