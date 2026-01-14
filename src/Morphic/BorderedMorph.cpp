#include "Woden/Morphic/BorderedMorph.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

namespace Woden
{
namespace Morphic
{

void BorderedMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    renderer->fillRectangleWithColorAndBorder(getLocalBounds(), getCurrentColor(), getCurrentBorderColor(), borderWidth);
}

} // End of namespace Morphic
} // End of namespace Woden