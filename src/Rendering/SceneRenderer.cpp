#include "Woden/Rendering/SceneRenderer.hpp"
#include "Woden/Rendering/Context.hpp"

namespace Woden
{
namespace Rendering
{

void SceneRenderer::renderScene(const SceneGraph::ScenePtr &scene)
{
}

void SceneRenderer::setupWithScreenSize(int newScreenWidth, int newScreenHeight)
{
    if(screen && screen->screenHeight == newScreenHeight && screen->screenWidth == newScreenWidth)
        return;

    // Ensure to finish the execution, so that we can release existing resources.
    auto device = RenderingContext::getMainContext()->device;
    device->finishExecution();

    // Create the new screen;
    screen = std::make_shared<SceneRendererScreen> ();
    screen->screenWidth = newScreenWidth;
    screen->screenHeight = newScreenHeight;

    // Depth stencil buffers
    printf("Creating depth-stencil buffer.\n");
    {
        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = newScreenWidth;
        desc.height = newScreenHeight;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = DepthStencilBufferFormat;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_DEPTH_ATTACHMENT | AGPU_TEXTURE_USAGE_STENCIL_ATTACHMENT | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.sample_count = 1;
        screen->depthStencilBuffer = device->createTexture(&desc);
        if(!screen->depthStencilBuffer)
        {
            fprintf(stderr, "Failed to create depth stencil buffer\n");
            abort();
        }
    }
}

} // End of namespace Rendering
} // End of namespace Woden