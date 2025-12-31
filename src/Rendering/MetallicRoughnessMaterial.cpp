#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"
#include "Woden/Rendering/SceneShaderCommon.hpp"

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
    sceneRenderer->currentCommandList->useShaderResources(getValidResourceBinding(sceneRenderer));
    return true;
}

bool MetallicRoughnessMaterial::activateOpaqueWithRenderer(SceneRenderer *sceneRenderer)
{
    if(alphaMode == SurfaceAlphaMode::Blend)
        return false;

    auto context = RenderingContext::getMainContext();
    sceneRenderer->currentCommandList->usePipelineState(context->metallicRoughnessOpaqueScenePipelineState);
    sceneRenderer->currentCommandList->useShaderResources(getValidResourceBinding(sceneRenderer));
    return true;
}

bool MetallicRoughnessMaterial::activateTranslucentWithRenderer(SceneRenderer *sceneRenderer)
{
    if(alphaMode != SurfaceAlphaMode::Blend)
        return false;

    // TODO: use translucent metallic-roughness pipeline.
    return false;
}

agpu_shader_resource_binding_ref MetallicRoughnessMaterial::getValidResourceBinding(SceneRenderer *sceneRenderer)
{
    if(materialResourceBinding && !hasChanged)
        return materialResourceBinding;

    const auto &context = RenderingContext::getMainContext();
    if(!materialResourceBinding)
    {
        materialResourceBinding = context->sceneShaderSignature->createShaderResourceBinding(2);
        materialState = reinterpret_cast<SceneMetallicRoughnessMaterial*> (sceneRenderer->allocateMaterialStateBuffer(sizeof(SceneMetallicRoughnessMaterial), materialResourceBinding));
    }

    materialState->baseColorFactor = baseColorFactor;
    materialState->emissiveFactor = Math::CompactVector3(emissiveFactor);
    materialState->occlusionFactor = occlusionFactor;

    materialState->roughnessFactor = roughnessFactor;
    materialState->metallicFactor = metallicFactor;

    materialState->texcoordOffset = texcoordOffset;
    materialState->texcoordScale = texcoordScale;
    materialState->texcoordOffsetVelocity = texcoordOffsetVelocity;

    materialResourceBinding->bindSampledTextureView(1, (baseColorTexture ? baseColorTexture->getValidTextureHandle() : context->whiteTexture)->getOrCreateFullView());
    materialResourceBinding->bindSampledTextureView(2, (emissiveTexture ? emissiveTexture->getValidTextureHandle() : context->whiteTexture)->getOrCreateFullView());
    materialResourceBinding->bindSampledTextureView(3, (normalTexture ? normalTexture->getValidTextureHandle() : context->neutralNormalTexture)->getOrCreateFullView());
    materialResourceBinding->bindSampledTextureView(4, (occlusionTexture ? occlusionTexture->getValidTextureHandle() : context->whiteTexture)->getOrCreateFullView());
    materialResourceBinding->bindSampledTextureView(5, (metallicRoughnessTexture ? metallicRoughnessTexture->getValidTextureHandle() : context->whiteTexture)->getOrCreateFullView());

    hasChanged = false;
    return materialResourceBinding;
}

} // End of namespace Rendering
} // End of namespace Woden