#include "Woden/Rendering/RenderingScene.hpp"

namespace Woden
{
namespace Rendering
{

void RenderingScene::addObjectWithRenderable(const RenderablePtr &renderable)
{
    RenderingSceneObject sceneObject = {};
    sceneObject.renderable = renderable;
    opaqueObjects.push_back(sceneObject);
}

} // End of namespace Rendering
} // End of namespace Woden