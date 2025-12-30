#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"

namespace Woden
{
namespace Rendering
{
bool MetallicRoughnessMaterial::activateDepthOnlyWithRenderer(SceneRenderer *sceneRenderer)
{
    if(alphaMode == SurfaceAlphaMode::Blend)
        return false;

    auto context = RenderingContext::getMainContext();
    sceneRenderer->currentCommandList->usePipelineState(context->depthOnlyScenePipelineState);
    return true;
}

bool MetallicRoughnessMaterial::activateOpaqueWithRenderer(SceneRenderer *sceneRenderer)
{
    if(alphaMode == SurfaceAlphaMode::Blend)
        return false;

    auto context = RenderingContext::getMainContext();
    sceneRenderer->currentCommandList->usePipelineState(context->metallicRoughnessOpaqueScenePipelineState);
    return true;
}

bool MetallicRoughnessMaterial::activateTranslucentWithRenderer(SceneRenderer *sceneRenderer)
{
    if(alphaMode != SurfaceAlphaMode::Blend)
        return false;

    // TODO: use translucent metallic-roughness pipeline.
    return false;
}

} // End of namespace Rendering
} // End of namespace Woden