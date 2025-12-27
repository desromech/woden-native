#include "Woden/Morphic/Morph.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

namespace Woden
{
namespace Morphic
{

Morph::Morph()
{
}

Morph::~Morph()
{
}

void Morph::fullDrawWith(const Rendering::GUIRendererPtr &renderer)
{
    drawWith(renderer);
    drawChildrenWith(renderer);
}

void Morph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    renderer->fillRectangleWithColor(getLocalBounds(), color);
}

void Morph::drawChildrenWith(const Rendering::GUIRendererPtr &renderer)
{
    for(auto &submorph : submorphs)
        submorph->fullDrawWith(renderer);
}

} // End of namespace Morphic
} // End of namespace Woden