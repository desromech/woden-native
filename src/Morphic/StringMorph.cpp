#include "Woden/Morphic/StringMorph.hpp"
#include "Woden/Rendering/Context.hpp"
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
    //renderer->fillRectangleWithColor(getLocalBounds(), Vector4(0, 1, 0, 1));
    renderer->drawTextInRectangleWithColor(getLocalBounds(), label, color);
}

void StringMorph::fitSize()
{
    auto fontFace = Woden::Rendering::RenderingContext::getMainContext()->defaultFontFace;
    setExtent(fontFace->measureTextExtent(label));

}

} // End of namespace Morphic
} // End of namespace Woden