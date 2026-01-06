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

SceneGraph::SceneNodePtr CollisionObject::constructVisualizationSceneNode()
{
    auto shapeNode = shape->constructVisualizationSceneNode();
    if(shapeNode)
        shapeNode->transform = transform.asTRSTransform3D();
    return shapeNode;
}

} // End of namespace Physics
} // End of namespace Woden