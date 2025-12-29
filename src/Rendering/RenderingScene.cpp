#include "Woden/Rendering/RenderingScene.hpp"

namespace Woden
{
namespace Rendering
{

void RenderingScene::addObjectWithRenderable(const RenderablePtr &renderable)
{
    RenderingSceneObject sceneObject = {};
    sceneObject.renderable = renderable;
    sceneObject.modelMatrix = currentModelMatrix;
    sceneObject.inverseModelMatrix = currentInverseModelMatrix;
    opaqueObjects.push_back(sceneObject);
}

} // End of namespace Rendering
} // End of namespace Woden