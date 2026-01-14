#ifndef WODEN_MORPHIC_MENU_BAR_MORPH_HPP
#define WODEN_MORPHIC_MENU_BAR_MORPH_HPP

#include "BorderedMorph.hpp"
#include <functional>

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class MenuItemMorph> MenuItemMorphPtr;
typedef std::shared_ptr<class MenuMorph> MenuMorphPtr;
typedef std::shared_ptr<class MenuBarMorph> MenuBarMorphPtr;

typedef std::shared_ptr<class HorizontalPackingLayout> HorizontalPackingLayoutPtr;
typedef std::shared_ptr<class VerticalPackingLayout> VerticalPackingLayoutPtr;

typedef std::function<void (MorphPtr)> ClickedAction;

class MenuItemMorph : public Morph
{
public:
    MenuItemMorph();

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;

    virtual void handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event) override;
    virtual void handleMouseMotionEvent(const MouseMotionEventPtr &event) override;

    void setLabel(const std::string &newLabel);

    MenuMorphPtr submenu;
    ClickedAction onClickAction;
protected:
    std::string label;
};

class MenuBarMorph : public BorderedMorph
{
public:
    MenuBarMorph();

    void addItem(const std::string &label, const MenuMorphPtr &submenu);

    virtual bool isMenuBar() const override;

    HorizontalPackingLayoutPtr barLayout;
    std::vector<MenuItemMorphPtr> items;
};

class MenuMorph : public BorderedMorph
{
public:
    MenuMorph();

    void addItem(const std::string &label, const MenuMorphPtr &submenu);
    void addItem(const std::string &label, const ClickedAction &onClickAction);

    VerticalPackingLayoutPtr menuLayout; 

};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_MENU_BAR_MORPH_HPP