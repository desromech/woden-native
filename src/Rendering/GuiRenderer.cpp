#include "Woden/Rendering/GuiRenderer.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Assets/Image.hpp"

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
        desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
        desc.usage_modes = desc.main_usage_mode = AGPU_STORAGE_BUFFER;
        desc.mapping_flags = AGPU_MAP_DYNAMIC_STORAGE_BIT;
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

void GUIRenderer::addGuiElementWithImage(const GuiElement &element, const Assets::ImagePtr &image)
{
    addGuiElementWithBinding(element, image->getValidGuiTextureBinding());
}

void GUIRenderer::addGuiElementWithBinding(const GuiElement &element, const agpu_shader_resource_binding_ref &newTextureBinding)
{
    currentTextureBinding = newTextureBinding;
    addGuiElement(element);
}

Math::Vector2 GUIRenderer::drawTextWithFaceAndColor(const Math::Vector2 &initialBaseline, const char *text, size_t textSize, const Assets::FontFacePtr &fontFace, const Math::Vector4 &color)
{
    auto currentBaseline = initialBaseline;
    // TODO: Decode UTF8
    for(size_t i = 0; i < textSize; ++i)
    {
        auto c = text[i];
        if(c < ' ')
            continue;

        stbtt_aligned_quad quadToDraw = {};
        stbtt_GetBakedQuad(fontFace->bakedChars.data(), fontFace->image->width, fontFace->image->height, c - 31, &currentBaseline.x, &currentBaseline.y, &quadToDraw, true);

        GuiElement quad = {};
        quad.type = GuiElementType::TextCharacter;

        quad.rectangleMin.x = quadToDraw.x0;
        quad.rectangleMin.y = quadToDraw.y0;
        quad.rectangleMax.x = quadToDraw.x1;
        quad.rectangleMax.y = quadToDraw.y1;

        quad.sourceImageRectangleMin.x = quadToDraw.s0;
        quad.sourceImageRectangleMin.y = quadToDraw.t0;
        quad.sourceImageRectangleMax.x = quadToDraw.s1;
        quad.sourceImageRectangleMax.y = quadToDraw.t1;

        quad.firstColor = color;
        addGuiElementWithImage(quad, fontFace->image);
    }

    return currentBaseline;
}

Math::Vector2 GUIRenderer::drawTextInRectangleWithColor(const Math::Rectangle &rectangle, const std::string &text, const Math::Vector4 &color)
{
    auto fontFace = RenderingContext::getMainContext()->defaultFontFace;
    auto initialBaseline = Math::Vector2(rectangle.minCorner.x, rectangle.minCorner.y + fontFace->ascent);
    return drawTextWithFaceAndColor(initialBaseline, text.data(), text.size(), fontFace, color);
}

Math::Vector2 GUIRenderer::drawTextWithColor(const Math::Vector2 &initialBaseline, const std::string &text, const Math::Vector4 &color)
{
    return drawTextWithFaceAndColor(initialBaseline, text.data(), text.size(), RenderingContext::getMainContext()->defaultFontFace, color);
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

void GUIRenderer::fillRectangleWithColorAndBorder(const Math::Rectangle &rectangle, const Math::Vector4 &color, const Math::Vector4 &borderColor, const Math::Scalar borderWidth)
{
    
    GuiElement element = {};
    element.type = GuiElementType::SolidRectangle;
    element.rectangleMin = rectangle.minCorner;
    element.rectangleMax = rectangle.maxCorner;
    element.firstColor = color;
    element.borderColor = borderColor;
    element.borderSize = borderWidth;
    addGuiElement(element);
}

void GUIRenderer::uploadDataWithCommandList(const agpu_command_list_ref &commandList)
{
    (void)commandList;
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
    commandList->useShaderResources(currentTextureBinding);
    commandList->pushConstants(0, sizeof(pushConstants), &pushConstants);
    commandList->drawArrays(4, guiElements.size(), 0, 0);
}

void GUIRenderer::reset()
{
    guiElements.clear();
    currentTextureBinding = RenderingContext::getMainContext()->guiEmptyTextureBinding;
}

} // End of namespace Rendering
} // End of namespace Woden
