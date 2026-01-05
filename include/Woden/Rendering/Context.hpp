#ifndef WODEN_RENDERING_CONTEXT_HPP
#define WODEN_RENDERING_CONTEXT_HPP

#include "Woden/Assets/Font.hpp"
#include "AGPU/agpu.hpp"
#include <memory>

namespace Woden
{
namespace Rendering
{

typedef std::shared_ptr<class RenderingContext> RenderingContextPtr;
/**
 * I hold a specific rendering context
 */
class RenderingContext
{
public:
    static const agpu_texture_format WindowColorBufferFormat = AGPU_TEXTURE_FORMAT_B8G8R8A8_UNORM_SRGB;

    static const agpu_texture_format HDRColorBufferFormat = AGPU_TEXTURE_FORMAT_R16G16B16A16_FLOAT;
    static const agpu_texture_format NormalGBufferFormat = AGPU_TEXTURE_FORMAT_R16G16_FLOAT;
    static const agpu_texture_format SpecularGBufferFormat = AGPU_TEXTURE_FORMAT_R8G8B8A8_UNORM;

    static const agpu_texture_format DepthStencilBufferFormat = AGPU_TEXTURE_FORMAT_R32G8X24_TYPELESS;
    static const agpu_texture_format DepthStencilBufferViewFormat = AGPU_TEXTURE_FORMAT_D32_FLOAT_S8X24_UINT;
    static const agpu_texture_format DepthStencilBufferSampledFormat = AGPU_TEXTURE_FORMAT_R32_FLOAT_S8X24_TYPELESS;

    static const agpu_texture_format ShadowMapAtlasFormat = AGPU_TEXTURE_FORMAT_R32_TYPELESS;
    static const agpu_texture_format ShadowMapAtlasViewFormat = AGPU_TEXTURE_FORMAT_D32_FLOAT;
    static const agpu_texture_format ShadowMapAtlasSampledFormat = AGPU_TEXTURE_FORMAT_R32_FLOAT;

    static RenderingContextPtr getMainContext();
    static RenderingContextPtr getOrCreateMainContext(int argc, const char *argv[]);
    static void releaseMainContext();

    bool initialize(int argc, const char *argv[]);
    agpu_shader_ref compileShader(const std::string &shaderFileName, agpu_shader_type type);
    agpu_shader_ref compileShader(const std::string &sharedCommon, const std::string &shaderFileName, agpu_shader_type type);
    agpu_shader_ref compileShader(const std::string &sharedCommon, const std::string &sharedCommon2, const std::string &shaderFileName, agpu_shader_type type);

    bool vsyncDisabled = false;
    agpu_device_ref device;
    agpu_command_queue_ref defaultCommandQueue;

    // GUI Rendering.
    agpu_renderpass_ref windowRenderPass;

    agpu_shader_signature_ref guiShaderSignature;
    agpu_pipeline_state_ref guiPipelineState;

    agpu_sampler_ref linearSampler;
    agpu_sampler_ref nearestSampler;
    agpu_shader_resource_binding_ref guiSamplerBindings;

    agpu_texture_ref whiteTexture;
    agpu_texture_ref neutralNormalTexture;
    agpu_shader_resource_binding_ref guiEmptyTextureBinding;

    Assets::FontPtr defaultFont;
    Assets::FontFacePtr defaultFontFace;

    Assets::FontPtr defaultMonospacedFont;
    Assets::FontFacePtr defaultMonospacedFontFace;

    // Scene Rendering.
    agpu_sampler_ref linearMipmapLinearSampler;
    agpu_sampler_ref nearestSceneSampler;
    agpu_sampler_ref shadowSampler;
    agpu_shader_resource_binding_ref sceneSamplerBindings;

    agpu_renderpass_ref depthStencilRenderPass;
    agpu_renderpass_ref shadowMapRenderPass;
    agpu_renderpass_ref hdrOpaqueRenderPass;

    agpu_pipeline_state_ref clearDepthPipelineState;

    agpu_vertex_layout_ref staticVertexLayout;
    agpu_pipeline_state_ref depthOnlyScenePipelineState;
    agpu_pipeline_state_ref depthOnlySceneCulledPipelineState;

    agpu_pipeline_state_ref shadowScenePipelineState;
    agpu_pipeline_state_ref shadowSceneCulledPipelineState;

    agpu_pipeline_state_ref metallicRoughnessOpaqueScenePipelineState;

    agpu_pipeline_state_ref transformLightsToViewPipeline;
    agpu_pipeline_state_ref lightGridComputationPipeline;
    agpu_pipeline_state_ref lightClusterBeginComputationPipeline;
    agpu_pipeline_state_ref lightClusterListComputationPipeline;

    agpu_shader_signature_ref sceneShaderSignature;

private:
    bool loadFonts();
    bool createGuiPipelineStates();
    bool createScenePipelineStates();

    static RenderingContextPtr mainContext;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_CONTEXT_HPP