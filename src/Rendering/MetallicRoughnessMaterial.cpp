#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
#include "Woden/Assets/ResourceLoadingContext.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"
#include "Woden/Rendering/SceneShaderCommon.hpp"

namespace Woden
{
namespace Rendering
{
static MaterialFactory<MetallicRoughnessMaterial> metallicRoughnessMaterialFactory("MetallicRoughness");

bool MetallicRoughnessMaterial::activateDepthOnlyWithRenderer(SceneRenderer *sceneRenderer)
{
    if(alphaMode == SurfaceAlphaMode::Blend)
        return false;

    auto context = RenderingContext::getMainContext();
    sceneRenderer->currentCommandList->usePipelineState(doubleSided ? context->depthOnlyScenePipelineState : context->depthOnlySceneCulledPipelineState);
    sceneRenderer->currentCommandList->useShaderResources(getValidResourceBinding(sceneRenderer));
    return true;
}

bool MetallicRoughnessMaterial::activateShadowMapWithRenderer(SceneRenderer *sceneRenderer)
{
    if(alphaMode == SurfaceAlphaMode::Blend)
        return false;

    auto context = RenderingContext::getMainContext();
    sceneRenderer->currentCommandList->usePipelineState(doubleSided ? context->shadowScenePipelineState : context->shadowSceneCulledPipelineState);
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
    (void)sceneRenderer;
    if(alphaMode != SurfaceAlphaMode::Blend)
        return false;

    // TODO: use translucent metallic-roughness pipeline.
    return false;
}

void MetallicRoughnessMaterial::loadWithContext(Assets::ResourceLoadingContext &context)
{
    SurfaceMaterial::loadWithContext(context);

    baseColorFactor = context.getVector4("baseColorFactor", baseColorFactor);
    emissiveFactor  = context.getVector3("emissiveFactor", emissiveFactor);

    occlusionFactor  = context.getFloat("occlusionFactor", occlusionFactor);
    roughnessFactor  = context.getFloat("roughnessFactor", roughnessFactor);
    metallicFactor   = context.getFloat("metallicFactor",  metallicFactor);

    texcoordOffset         = context.getVector2("texcoordOffset",         texcoordOffset);
    texcoordScale          = context.getVector2("texcoordScale",          texcoordScale);
    texcoordOffsetVelocity = context.getVector2("texcoordOffsetVelocity", texcoordOffsetVelocity);

    
    baseColorTexture         = context.getTextureMember("baseColorTexture",         Assets::TextureUsageMode::Color);
    emissiveTexture          = context.getTextureMember("emissiveTexture",          Assets::TextureUsageMode::Color);
    normalTexture            = context.getTextureMember("normalTexture",            Assets::TextureUsageMode::Normal);
    occlusionTexture         = context.getTextureMember("occlusionTexture",         Assets::TextureUsageMode::Data);
    metallicRoughnessTexture = context.getTextureMember("metallicRoughnessTexture", Assets::TextureUsageMode::Data);    
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

    materialState->doubleSided = doubleSided ? 1 : 0;
    materialState->alphaCutoff = alphaMode == SurfaceAlphaMode::Mask ? alphaCutoff : 0; 

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