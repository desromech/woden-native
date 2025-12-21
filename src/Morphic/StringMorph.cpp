#include "Woden/Morphic/StringMorph.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

namespace Woden
{
namespace Morphic
{

StringMorph::StringMorph()
{
    color = Vector4(0, 0, 0, 1);
}

void StringMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    renderer->drawTextInRectangleWithColor(getLocalBounds(), label, color);
}

} // End of namespace Morphic
} // End of namespace Woden