#include "Woden/Morphic/Layout.hpp"
#include "Woden/Morphic/Morph.hpp"

namespace Woden
{
namespace Morphic
{

void FillMorphicLayout::applyWithLocalBounds(const Math::Rectangle &localBounds)
{
    morph->setBounds(localBounds);
}

} // End of namespace Morphic
} // End of namespace Woden
