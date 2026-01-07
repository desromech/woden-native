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

} // End of namespace Physics
} // End of namespace Woden