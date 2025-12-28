#include "Woden/Rendering/MeshPrimitive.hpp"
#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/VertexBinding.hpp"

namespace Woden
{
namespace Rendering
{

void MeshPrimitive::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    renderingScene->addObjectWithRenderable(shared_from_this());
}

void MeshPrimitive::renderDepthOnlyWith(SceneRenderer *renderer) 
{
    auto context = RenderingContext::getMainContext();

    auto &commandList = renderer->currentCommandList;
    commandList->usePipelineState(context->depthOnlyScenePipelineState);
    vertexBinding->useWithRenderer(renderer);
    renderer->useIndexBuffer(indices);
    commandList->drawElements(indices->count, 1, 0, 0, 0);
}

void MeshPrimitive::renderOpaqueWith(SceneRenderer *renderer) 
{
}

} // End of namespace Rendering
} // End of namespace Woden