#include "Woden/Morphic/MenuBarMorph.hpp"
#include "Woden/Morphic/Layout.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

namespace Woden
{
namespace Morphic
{

MenuItemMorph::MenuItemMorph()
{
    color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
}

void MenuItemMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    if(hasMouseFocus())
    {
        renderer->fillRectangleWithColor(getLocalBounds(), Vector4(0.6f, 0.6f, 0.6f, 1.0f));
    }

    renderer->drawTextInRectangleWithColor(getLocalBounds().translatedBy(Vector2(5, 5)), label, Vector4(0, 0, 0, 1));
}

void MenuItemMorph::setLabel(const std::string &newLabel)
{
    label = newLabel;
    auto fontFace = Woden::Rendering::RenderingContext::getMainContext()->defaultFontFace;
    setExtent(fontFace->measureTextExtent(label) + Vector2(10, 10));
}

void MenuItemMorph::handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    if(event->isLeftButton())
    {
    }
}

void MenuItemMorph::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    takeMouseFocus();
}


MenuBarMorph::MenuBarMorph()
{
    color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
    barLayout = std::make_shared<HorizontalPackingLayout> ();
    setLayout(barLayout);
    setExtent(Vector2(100, 30));
}

void MenuBarMorph::addItem(const std::string &label, const MenuMorphPtr &submenu)
{
    auto item = std::make_shared<MenuItemMorph> ();
    item->setLabel(label);
    item->submenu = submenu;

    addMorph(item);
    barLayout->addMorph(item, 0, false);
    //updateLayout();
}

} // End of namespace Morphic
} // End of namespace Woden