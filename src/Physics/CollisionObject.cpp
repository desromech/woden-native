#include "Woden/Physics/CollisionObject.hpp"
#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace Physics
{
void CollisionObject::transformChanged()
{
    // Nothing required here for now.
}

Math::AABox CollisionObject::getWorldBoundingBoxWithMargin() const
{
    return shape->localBoundingBoxWithMargin.transformedWith(transform);
}

Math::Scalar CollisionObject::computeAngularInertiaForRelativeContactPoint(const Math::Vector3 &relativePoint, const Math::Vector3 &normal) const
{
    (void)relativePoint;
    (void)normal;
    return 0;
}

Math::Matrix3x3 CollisionObject::computeVelocityPerImpulseWorldMatrixForRelativeContactPoint(const Math::Vector3 &relativePoint) const
{
    (void)relativePoint;
    return Math::Matrix3x3::Zeros();
}

Math::Vector3 CollisionObject::computeVelocityAtRelativePoint(const Math::Vector3 &relativePoint)
{
    (void)relativePoint;
    return Math::Vector3::Zeros();
}

SceneGraph::SceneNodePtr CollisionObject::constructVisualizationSceneNode()
{
    auto shapeNode = shape->constructVisualizationSceneNode();
    if(shapeNode)
        shapeNode->transform = transform.asTRSTransform3D();
    return shapeNode;
}

void CollisionObject::resetNetForces()
{
}

void CollisionObject::integrateMovement(Math::Scalar deltaTime)
{
    (void)deltaTime;
}

bool CollisionObject::needsCollisionDetection()
{
    return false;
}

Math::Scalar CollisionObject::getMass() const
{
    return 0;
}

Math::Scalar CollisionObject::getInverseMass() const
{
    return 0;
}

void CollisionObject::applyForceInRelativePosition(const Math::Vector3 &force, const Math::Vector3 &relativePosition)
{
    (void)force;
    (void)relativePosition;
    // BY default do nothing
}

void CollisionObject::applyForceInPosition(const Math::Vector3 &force, const Math::Vector3 &worldPosition)
{
    applyForceInRelativePosition(force, worldPosition - transform.translation);
}

void CollisionObject::applyMovementAtRelativePoint(Math::Scalar movement, const Math::Vector3 &relativePoint, const Math::Vector3 &normalDirection)
{
    (void)movement;
    (void)relativePoint;
    (void)normalDirection;
}

void CollisionObject::applyImpulse(const Math::Vector3 &impulse)
{
    (void)impulse;
}

void CollisionObject::applyImpulseInRelativePosition(const Math::Vector3 &impulse, const Math::Vector3 &relativePoint)
{
    (void)relativePoint;
    applyImpulse(impulse);
}

void CollisionObject::translateBy(const Math::Vector3 &translation)
{
    transform.translation += translation;
    transformChanged();
}

void CollisionObject::translateByAndRotateBy(const Math::Vector3 &translation, const Math::Vector3 &angularIncrement)
{
    transform.translation += translation;
    transform.rotation = (Math::Quaternion(angularIncrement * Math::Vector3(0.5)).exp() * transform.rotation).normalized();
    transformChanged();
}

void CollisionObject::loadStateFromModel()
{
    if(model)
        model->loadCollisionStateInto(shared_from_this());
}

void CollisionObject::saveStateIntoModel()
{
    if(model)
        model->saveCollisionStateFrom(shared_from_this());
}

void CollisionObject::resetSleepingState()
{
}

void CollisionObject::setInternalLinearAcceleration(const Math::Vector3 &acceleration)
{
    internalLinearAcceleration = acceleration;
    if(internalLinearAcceleration.length2() > 0)
        wakeUp();
}

bool CollisionObject::isOnFloor()
{
    if(isSleeping())
        return true;

    if(contactManifolds.empty())
        return false;

    // Find a contact point that is below me.
    for (auto &manifold : contactManifolds)
    {
        //printf("Manifold\n");
        for (auto &contactPoint : manifold->contacts)
        {
            Math::Vector3 normal = contactPoint.normal;
            if(contactPoint.secondObject.get() == this)
                normal = -contactPoint.normal;
            
            Math::Vector3 up = Math::Vector3(0, 1, 0);
            auto NdotUp = normal.dot(up);
            //printf("N %f %f %f NdotUp: %f\n", normal.x, normal.y, normal.z, NdotUp);
            if(NdotUp >= 0.25)
                return true;
        }
    }

    return false;
}

void CollisionObject::jump(const Math::Vector3 &jumpVelocity)
{
    if(!isOnFloor())
        return;
    
    linearVelocity += jumpVelocity;
    wakeUp();
}

void CollisionObject::wakeUp()
{
}

std::optional<ShapeRayCastingResult> CollisionObject::rayCast(const Math::Ray3D &ray)
{
    auto localStartPoint = transform.inverseTransformPosition(ray.getStartPoint());
    auto localEndPoint = transform.inverseTransformPosition(ray.getEndPoint());
    auto localRay = Math::Ray3D::FromTo(localStartPoint, localEndPoint);
    
    auto result = shape->rayCast(localRay);
    if(!result.has_value())
        return std::nullopt;

    auto castingResult = result.value();
    castingResult.collisionObject = shared_from_this();
    castingResult.shape = shape;
    return castingResult;
}

void CollisionObject::clearContactManifolds()
{
    contactManifolds.clear();
}

void CollisionObject::addContactManifold(const ContactManifoldPtr &manifold)
{
    contactManifolds.push_back(manifold);
}

} // End of namespace Physics
} // End of namespace Woden