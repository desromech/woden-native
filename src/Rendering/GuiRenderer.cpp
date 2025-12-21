#include "Woden/Rendering/GuiRenderer.hpp"
#include "Woden/Rendering/Context.hpp"

namespace Woden
{
namespace Rendering
{

bool GUIRenderer::initialize()
{
    auto renderingContext = RenderingContext::getMainContext();
    guiElements.reserve(MaxSupportedGUIElements);

    {
        agpu_buffer_description desc = {};
        desc.size = agpu_uint(MaxSupportedGUIElements * sizeof(GuiElement));
        desc.heap_type = AGPU_MEMORY_HEAP_TYPE_HOST_TO_DEVICE;
        desc.usage_modes = desc.main_usage_mode = AGPU_STORAGE_BUFFER;
        desc.mapping_flags = AGPU_MAP_DYNAMIC_STORAGE_BIT | AGPU_MAP_WRITE_BIT;
        desc.stride = 0;
        
        guiElementsBuffer = renderingContext->device->createBuffer(&desc, nullptr);
    }

    {
        guiElementsBinding = renderingContext->guiShaderSignature->createShaderResourceBinding(1);
        guiElementsBinding->bindStorageBuffer(0, guiElementsBuffer);
    }

    return true;
}

void GUIRenderer::addGuiElement(const GuiElement &element)
{
    if(guiElements.size() >= MaxSupportedGUIElements)
    {
        fprintf(stderr, "Warning: Excessive GUI elements");
        return;
    }

    guiElements.push_back(element);
}

void GUIRenderer::fillRectangleWithColor(const Math::Rectangle &rectangle, const Math::Vector4 &color)
{
    GuiElement element = {};
    element.type = GuiElementType::SolidRectangle;
    element.rectangleMin = rectangle.minCorner;
    element.rectangleMax = rectangle.maxCorner;
    element.firstColor = color;
    addGuiElement(element);
}

void GUIRenderer::uploadDataWithCommandList(const agpu_command_list_ref &commandList)
{
    if(guiElements.empty())
        return;

    guiElementsBuffer->uploadBufferData(0, sizeof(GuiElement)*guiElements.size(), guiElements.data());
}

void GUIRenderer::drawOnCommandList(const agpu_command_list_ref &commandList)
{
    if(guiElements.empty())
        return;

    GUIPushConstants pushConstants = {};
    pushConstants.hasTopLeftNDCOrigin = RenderingContext::getMainContext()->device->hasTopLeftNdcOrigin() ? 1 : 0;
    pushConstants.framebufferReciprocalExtent = framebufferExtent.reciprocal();

    auto renderingContext = RenderingContext::getMainContext();
    commandList->setShaderSignature(renderingContext->guiShaderSignature);
    commandList->usePipelineState(renderingContext->guiPipelineState);
    commandList->useShaderResources(renderingContext->guiSamplerBindings);
    commandList->useShaderResources(guiElementsBinding);
    commandList->useShaderResources(renderingContext->guiEmptyTextureBinding);
    commandList->pushConstants(0, sizeof(pushConstants), &pushConstants);
    commandList->drawArrays(4, guiElements.size(), 0, 0);
}

void GUIRenderer::reset()
{
    guiElements.clear();
}

} // End of namespace Rendering
} // End of namespace Woden
