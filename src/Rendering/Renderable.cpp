#include "Woden/Rendering/Renderable.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace Rendering
{

void Renderable::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    (void)renderingScene;
}

SceneGraph::SceneNodePtr Renderable::asSceneNode()
{
    auto sceneNode = std::make_shared<SceneGraph::SceneNode> ();
    sceneNode->addRenderable(shared_from_this());
    return sceneNode;
}

} // End of namespace Rendering
} // End of namespace Woden