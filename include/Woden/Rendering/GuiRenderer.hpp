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

    void uploadDataWithCommandList(const agpu_command_list_ref &commandList);
    void drawOnCommandList(const agpu_command_list_ref &commandList);

    std::vector<GuiElement> guiElements;

    agpu_buffer_ref guiElementsBuffer;
    agpu_shader_resource_binding_ref guiElementsBinding;
    agpu_shader_resource_binding_ref currentTextureBinding;
    Math::Vector2 framebufferExtent;
    Math::Vector2 scaleFactor;

    agpu_command_list_ref renderingCommandList;
};

} // End of namespace Rendering
} // End of namespace Woden


#endif //WODEN_GUI_RENDERER_HPP