#ifndef WODEN_MORPHIC_LAYOUT_HPP
#define WODEN_MORPHIC_LAYOUT_HPP

#include <memory>

namespace Woden
{
namespace Morphic
{

typedef std::shared_ptr<class MorphicLayout> MorphicLayoutPtr;
typedef std::shared_ptr<class Morph> MorphPtr;

/**
 * Morphic layout abstract base class.
 */
class MorphicLayout
{
public:
};

/**
 * I am a layout that fills the parent morph with a single morph
 */
class FillMorphicLayout : public MorphicLayout
{
public:

    MorphPtr morph;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_LAYOUT_HPP