#include "Woden/Rendering/Context.hpp"
#include <vector>
#include <stdio.h>

namespace Woden
{
namespace Rendering
{

RenderingContextPtr RenderingContext::mainContext;

RenderingContextPtr RenderingContext::getMainContext()
{
    return mainContext;
}

RenderingContextPtr RenderingContext::getOrCreateMainContext(int argc, const char *argv[])
{
    if(mainContext)
        return mainContext;

    auto newContext = std::make_shared<RenderingContext> ();
    if(newContext->initialize(argc, argv))
        mainContext = newContext;

    return mainContext;
}

void RenderingContext::releaseMainContext()
{
    mainContext.reset();
}


bool RenderingContext::initialize(int argc, const char *argv[])
{
    bool debugLayerEnabled = false;
#ifdef _DEBUG
    debugLayerEnabled= true;
#endif
    agpu_uint platformIndex = 0;
    agpu_uint gpuIndex = 0;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-no-vsync")
        {
            vsyncDisabled = true;
        }
        else if (arg == "-platform")
        {
            platformIndex = agpu_uint(atoi(argv[++i]));
        }
        else if (arg == "-gpu")
        {
            gpuIndex = agpu_uint(atoi(argv[++i]));
        }
        else if (arg == "-debug")
        {
            debugLayerEnabled = true;
        }
    }

        // Get the platform.
    agpu_uint numPlatforms;
    agpuGetPlatforms(0, nullptr, &numPlatforms);
    if (numPlatforms == 0)
    {
        fprintf(stderr, "No agpu platforms are available.\n");
        return false;
    }
    else if (platformIndex >= numPlatforms)
    {
        fprintf(stderr, "Selected platform index is not available.\n");
        return false;
    }

    std::vector<agpu_platform*> platforms;
    platforms.resize(numPlatforms);
    agpuGetPlatforms(numPlatforms, &platforms[0], nullptr);
    auto platform = platforms[platformIndex];

    printf("Chosen platform: %s\n", agpuGetPlatformName(platform));

    agpu_device_open_info deviceOpenInfo = {};
    deviceOpenInfo.gpu_index = gpuIndex;
    deviceOpenInfo.debug_layer = debugLayerEnabled;

    device = platform->openDevice(&deviceOpenInfo);
    if(!device)
        return false;

    defaultCommandQueue = device->getDefaultCommandQueue();

    // Create the window render pass
    {
        // Color attachment
        agpu_renderpass_color_attachment_description colorAttachment = {};
        colorAttachment.format = WindowColorBufferFormat;
        colorAttachment.begin_action = AGPU_ATTACHMENT_CLEAR;
        colorAttachment.end_action = AGPU_ATTACHMENT_KEEP;
        colorAttachment.sample_count = 1;


        agpu_renderpass_description description = {};
        description.color_attachment_count = 1;
        description.color_attachments = &colorAttachment;

        windowRenderPass = device->createRenderPass(&description);
        if(!windowRenderPass)
            return false;
    }

    return true;
}

} // End of namespace Rendering
} // End of namespace Woden