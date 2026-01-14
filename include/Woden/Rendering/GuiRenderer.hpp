#ifndef WODEN_GUI_RENDERER_HPP
#define WODEN_GUI_RENDERER_HPP

#include "Woden/Assets/Font.hpp"
#include "Woden/Math/Rectangle.hpp"
#include "Woden/Math/Vector4.hpp"
#include "Woden/Rendering/GuiElement.hpp"
#include "AGPU/agpu.hpp"
#include <vector>

namespace Woden
{
namespace Rendering
{

/**
 * I am a bundle of GUI elements that share a specific texture binding.
 */
struct GuiElementSet
{
    size_t startIndex = 0;
    size_t elementCount = 0;
    agpu_shader_resource_binding_ref textureBinding;
};

class GUIRenderer 
{
public:
    size_t MaxSupportedGUIElements = 4096;

    bool initialize();
    void reset();

    void addGuiElement(const GuiElement &element);
    void addGuiElementWithImage(const GuiElement &element, const Assets::ImagePtr &image);
    void addGuiElementWithBinding(const GuiElement &element, const agpu_shader_resource_binding_ref &newTextureBinding);

    Math::Vector2 drawTextWithFaceAndColor(const Math::Vector2 &initialBaseline, const char *text, size_t textSize, const Assets::FontFacePtr &fontFace, const Math::Vector4 &color);
    Math::Vector2 drawTextInRectangleWithColor(const Math::Rectangle &rectangle, const std::string &text, const Math::Vector4 &color);
    Math::Vector2 drawTextWithColor(const Math::Vector2 &initialBaseline, const std::string &text, const Math::Vector4 &color);

    void fillRectangleWithColor(const Math::Rectangle &rectangle, const Math::Vector4 &color);
    void fillRectangleWithColorAndBorder(const Math::Rectangle &rectangle, const Math::Vector4 &color, const Math::Vector4 &borderColor, const Math::Scalar borderWidth);

    void uploadDataWithCommandList(const agpu_command_list_ref &commandList);
    void drawOnCommandList(const agpu_command_list_ref &commandList);

    template<typename FT>
    void withTranslationDo(const Math::Vector2 &translation, FT&& aBlock)
    {
        auto oldTranslation = currentTranslation;
        currentTranslation += translation;
        aBlock();
        currentTranslation = oldTranslation;
    }

    std::vector<GuiElement> guiElements;
    std::vector<GuiElementSet> guiElementsSets;

    agpu_buffer_ref guiElementsBuffer;
    agpu_shader_resource_binding_ref guiElementsBinding;
    Math::Vector2 framebufferExtent;
    Math::Vector2 scaleFactor;

    agpu_command_list_ref renderingCommandList;

    Math::Vector2 currentTranslation = Math::Vector2(0, 0);

};

} // End of namespace Rendering
} // End of namespace Woden


#endif //WODEN_GUI_RENDERER_HPP