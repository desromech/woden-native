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

void Renderable::renderShadowWith(SceneRenderer *renderer)
{
    (void)renderer;
}

void Renderable::renderDepthOnlyWith(SceneRenderer *renderer)
{
    (void)renderer;
}

void Renderable::renderOpaqueWith(SceneRenderer *renderer)
{
    (void)renderer;
}

void Renderable::renderTranslucentWith(SceneRenderer *renderer)
{
    (void)renderer;
}

SceneGraph::SceneNodePtr Renderable::asSceneNode()
{
    auto sceneNode = std::make_shared<SceneGraph::SceneNode> ();
    sceneNode->addRenderable(shared_from_this());
    return sceneNode;
}

} // End of namespace Rendering
} // End of namespace Woden