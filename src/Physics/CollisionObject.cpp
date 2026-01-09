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

} // End of namespace Physics
} // End of namespace Woden