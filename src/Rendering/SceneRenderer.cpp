#include "Woden/Rendering/SceneRenderer.hpp"
#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/Context.hpp"

namespace Woden
{
namespace Rendering
{

agpu_shader_resource_binding_ref SceneRendererScreen::getValidGuiTextureBinding()
{
    if(guiTextureBinding)
        return guiTextureBinding;

    guiTextureBinding = Rendering::RenderingContext::getMainContext()->guiShaderSignature->createShaderResourceBinding(2);
    guiTextureBinding->bindSampledTextureView(0, hdrColorBuffer->getOrCreateFullView());
    return guiTextureBinding;
}

void SceneRenderer::renderScene(const agpu_command_list_ref &commandList, const SceneGraph::ScenePtr &scene)
{
    auto context = RenderingContext::getMainContext();
    currentCommandList = commandList;

    currentRenderingScene = std::make_shared<RenderingScene> ();
    scene->addIntoRenderingScene(currentRenderingScene);

    // Depth-Stencil render pass.
    commandList->beginRenderPass(context->depthStencilRenderPass, screen->depthOnlyFramebuffer, false);
    commandList->setViewport(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setScissor(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setShaderSignature(context->sceneShaderSignature);

    performDepthOnlyPass();

    commandList->endRenderPass();

    // HDR opaque render pass.
    commandList->beginRenderPass(context->hdrOpaqueRenderPass, screen->hdrOpaqueFramebuffer, false);
    commandList->setViewport(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setScissor(0, 0, screen->screenWidth, screen->screenHeight);

    performHDROpaquePass();

    commandList->endRenderPass();
    currentCommandList.reset();
}


void SceneRenderer::performDepthOnlyPass()
{
    for(auto &object : currentRenderingScene->opaqueObjects)
        object.renderable->renderDepthOnlyWith(this);
}

void SceneRenderer::performHDROpaquePass()
{
    for(auto &object : currentRenderingScene->opaqueObjects)
        object.renderable->renderOpaqueWith(this);
}

void SceneRenderer::useIndexBuffer(const Assets::BinaryBufferAccessorPtr &indices)
{
    auto bufferHandle = indices->bufferView->buffer->getOrCreateBufferWithContents();
    currentCommandList->useIndexBufferAt(
        bufferHandle,
        indices->bufferView->byteOffset + indices->byteOffset,
        indices->bufferView->byteStride
    );
    
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
    {
        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = newScreenWidth;
        desc.height = newScreenHeight;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = RenderingContext::DepthStencilBufferFormat;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_DEPTH_ATTACHMENT | AGPU_TEXTURE_USAGE_STENCIL_ATTACHMENT | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.sample_count = 1;
        desc.clear_value.depth_stencil.depth = 0;
        screen->depthStencilBuffer = device->createTexture(&desc);
        if(!screen->depthStencilBuffer)
        {
            fprintf(stderr, "Failed to create depth stencil buffer\n");
            abort();
        }
    }

    {
        agpu_texture_view_description viewDesc = {};
        screen->depthStencilBuffer->getFullViewDescription(&viewDesc);
        viewDesc.format = RenderingContext::DepthStencilBufferViewFormat;

        screen->depthStencilAttachmentView = screen->depthStencilBuffer->createView(&viewDesc);
        if(!screen->depthStencilAttachmentView)
        {
            fprintf(stderr, "Failed to create depth stencil buffer view\n");
            abort();
        }
    }
    
    // HDR Color Buffer
    {
        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = newScreenWidth;
        desc.height = newScreenHeight;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = RenderingContext::HDRColorBufferFormat;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_COLOR_ATTACHMENT | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.sample_count = 1;
        desc.clear_value.depth_stencil.depth = 0;
        screen->hdrColorBuffer = device->createTexture(&desc);
        if(!screen->hdrColorBuffer)
        {
            fprintf(stderr, "Failed to create HDR color buffer\n");
            abort();
        }

        agpu_texture_view_description attachmentViewDesc = {};
        screen->hdrColorBuffer->getFullViewDescription(&attachmentViewDesc);
        attachmentViewDesc.usage_mode = AGPU_TEXTURE_USAGE_COLOR_ATTACHMENT;

        screen->hdrColorBufferView = screen->hdrColorBuffer->createView(&attachmentViewDesc);
    }

    // Depth-Stencil only framebuffer
    {
        screen->depthOnlyFramebuffer = device->createFrameBuffer(newScreenWidth, newScreenHeight, 0, nullptr, screen->depthStencilAttachmentView);
        if(!screen->depthOnlyFramebuffer)
        {
            fprintf(stderr, "Failed to create depth only framebuffer\n");
            abort();
        }
    }

    // HDR-depth framebuffer
    {
        screen->hdrOpaqueFramebuffer = device->createFrameBuffer(newScreenWidth, newScreenHeight, 1, &screen->hdrColorBufferView, screen->depthStencilAttachmentView);
        if(!screen->depthOnlyFramebuffer)
        {
            fprintf(stderr, "Failed to create hdr-opaque framebuffer\n");
            abort();
        }
    }

}

} // End of namespace Rendering
} // End of namespace Woden