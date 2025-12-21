#ifndef WODEN_GUI_RENDERER_HPP
#define WODEN_GUI_RENDERER_HPP

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
    void fillRectangleWithColor(const Math::Rectangle &rectangle, const Math::Vector4 &color);

    void uploadDataWithCommandList(const agpu_command_list_ref &commandList);
    void drawOnCommandList(const agpu_command_list_ref &commandList);

    std::vector<GuiElement> guiElements;

    agpu_buffer_ref guiElementsBuffer;
    agpu_shader_resource_binding_ref guiElementsBinding;
    Math::Vector2 framebufferExtent;

};

} // End of namespace Rendering
} // End of namespace Woden


#endif //WODEN_GUI_RENDERER_HPP