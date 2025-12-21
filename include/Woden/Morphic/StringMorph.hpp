#ifndef WODEN_MORPHIC_STRING_MORPH_HPP
#define WODEN_MORPHIC_STRING_MORPH_HPP

#include "Morph.hpp"

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class StringMorph> StringMorphPtr;

/**
 * I am morph
 */
class StringMorph : public Morph
{
public:
    StringMorph();

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;

    std::string label;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_STRING_MORPH_HPP