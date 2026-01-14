#ifndef WODEN_MORPHIC_MENU_BAR_MORPH_HPP
#define WODEN_MORPHIC_MENU_BAR_MORPH_HPP

#include "BorderedMorph.hpp"

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class MenuBarMorph> MenuBarMorphPtr;

class MenuBarMorph : public BorderedMorph
{
public:
    MenuBarMorph()
    {
        color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
        setExtent(Vector2(100, 30));
    }
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_MENU_BAR_MORPH_HPP