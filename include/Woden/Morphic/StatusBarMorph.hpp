#ifndef WODEN_MORPHIC_STATUS_BAR_MORPH_HPP
#define WODEN_MORPHIC_STATUS_BAR_MORPH_HPP

#include "BorderedMorph.hpp"

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class StatusBarMorph> StatusBarMorphPtr;

class StatusBarMorph : public BorderedMorph
{
public:
    StatusBarMorph()
    {
        color = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
        setExtent(Vector2(100, 30));
    }
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_STATUS_BAR_MORPH_HPP