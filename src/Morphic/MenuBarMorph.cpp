#include "Woden/Morphic/MenuBarMorph.hpp"
#include "Woden/Morphic/RootMorph.hpp"
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
        if(owner.lock()->isMenuBar())
            renderer->fillRectangleWithColor(getLocalBounds(), Vector4(0.6f, 0.6f, 0.6f, 1.0f));
        else
            renderer->fillRectangleWithColor(getLocalBounds(), Vector4(0.2f, 0.2f, 0.8f, 1.0f));
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
        auto ownerMorph = owner.lock();
        if(ownerMorph && ownerMorph->isMenuBar())
        {
            auto menuBar = std::static_pointer_cast<MenuBarMorph> (ownerMorph);
            menuBar->toggleSubmenuOpenAtPosition(submenu, transformLocalPositionToGlobal(Vector2(0, getExtent().y)));
        }
        else
        {
            auto rootMorph = getRootMorph();
            if(rootMorph)
                rootMorph->killPopUps();
        }

        // Send the action.
        if(onClickAction)
            onClickAction(shared_from_this());

        event->wasHandled = true;

    }
}

void MenuItemMorph::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    takeMouseFocus();
    event->wasHandled = true;

    auto ownerMorph = owner.lock();
    if(ownerMorph && ownerMorph->isMenuBar() && ownerMorph->isMenuOpen() && submenu)
    {
        ownerMorph->getRootMorph()->popupUniqueAtPosition(submenu, transformLocalPositionToGlobal(Vector2(0, getExtent().y)));
    }

}


MenuBarMorph::MenuBarMorph()
{
    color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
    borderWidth = 0;
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


bool MenuBarMorph::isMenuBar() const
{
    return true;
}

bool MenuBarMorph::isMenuOpen() const
{
    return isMenuOpen_;
}

void MenuBarMorph::toggleSubmenuOpenAtPosition(const MenuMorphPtr &submenu, const Vector2 &openPosition)
{
    if(isMenuOpen_)
    {
        isMenuOpen_ = false;
        getRootMorph()->killPopUps();
    }
    else
    {
        isMenuOpen_ = true;
        getRootMorph()->popupUniqueAtPosition(submenu, openPosition);
    }
}

MenuMorph::MenuMorph()
{
    color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
    menuLayout = std::make_shared<VerticalPackingLayout> ();
    setLayout(menuLayout);
    setExtent(Vector2(100, 30));
}

void MenuMorph::addItem(const std::string &label, const MenuMorphPtr &submenu)
{
    auto item = std::make_shared<MenuItemMorph> ();
    item->setLabel(label);
    item->submenu = submenu;

    addMorph(item);
    menuLayout->addMorph(item, 0, false);
    
    fitSize();
}

void MenuMorph::addItem(const std::string &label, const ClickedAction &onClickAction)
{
    auto item = std::make_shared<MenuItemMorph> ();
    item->setLabel(label);
    item->onClickAction = onClickAction;

    addMorph(item);
    menuLayout->addMorph(item, 0, true);

    fitSize();
}

void MenuMorph::fitSize()
{
    Vector2 fitExtent = Vector2(0);
    for(auto &morph : submorphs)
    {
        auto extent = morph->getExtent();
        fitExtent.x = std::max(fitExtent.x, extent.x);
        fitExtent.y += extent.y;
    }

    setExtent(fitExtent);
}

} // End of namespace Morphic
} // End of namespace Woden