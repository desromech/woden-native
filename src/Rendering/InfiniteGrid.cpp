#include "Woden/Rendering/InfiniteGrid.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"

namespace Woden
{
namespace Rendering
{

void InfiniteGridRenderable::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    renderingScene->addTranslucentObjectWithRenderable(shared_from_this());
}

void InfiniteGridRenderable::renderTranslucentWith(SceneRenderer *renderer)
{
    auto pso = RenderingContext::getMainContext()->infiniteGridPipelineState;
    renderer->currentCommandList->usePipelineState(pso);
    renderer->currentCommandList->drawArrays(4, 1, 0, 0);
}


} // End of namespace Rendering
} // End of namespace Woden
