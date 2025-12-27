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

    static RenderingContextPtr getMainContext();
    static RenderingContextPtr getOrCreateMainContext(int argc, const char *argv[]);
    static void releaseMainContext();

    bool initialize(int argc, const char *argv[]);
    agpu_shader_ref compileShader(const std::string &sharedCommon, const std::string &shaderFileName, agpu_shader_type type);

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
    agpu_shader_resource_binding_ref guiEmptyTextureBinding;

    Assets::FontPtr defaultFont;
    Assets::FontFacePtr defaultFontFace;

    Assets::FontPtr defaultMonospacedFont;
    Assets::FontFacePtr defaultMonospacedFontFace;

    // Scene Rendering.
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