#ifndef WODEN_MORPHIC_MENU_BAR_MORPH_HPP
#define WODEN_MORPHIC_MENU_BAR_MORPH_HPP

#include "BorderedMorph.hpp"

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class MenuItemMorph> MenuItemMorphPtr;
typedef std::shared_ptr<class MenuMorph> MenuMorphPtr;
typedef std::shared_ptr<class MenuBarMorph> MenuBarMorphPtr;

typedef std::shared_ptr<class HorizontalPackingLayout> HorizontalPackingLayoutPtr;

class MenuItemMorph : public Morph
{
public:
    MenuItemMorph();

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;

    virtual void handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event) override;
    virtual void handleMouseMotionEvent(const MouseMotionEventPtr &event) override;

    void setLabel(const std::string &newLabel);

    MenuMorphPtr submenu;
protected:
    std::string label;
};

class MenuBarMorph : public BorderedMorph
{
public:
    MenuBarMorph();

    void addItem(const std::string &label, const MenuMorphPtr &submenu);

    HorizontalPackingLayoutPtr barLayout;
    std::vector<MenuItemMorphPtr> items;
};

class MenuMorph : public BorderedMorph
{
public:
    MenuMorph()
    {
        color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
        setExtent(Vector2(100, 30));
    }
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_MENU_BAR_MORPH_HPP