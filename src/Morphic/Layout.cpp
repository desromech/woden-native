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

void CenterMorphicLayout::applyWithLocalBounds(const Math::Rectangle &localBounds)
{
    auto childExtent = morph->getExtent();
    auto localCenter = localBounds.extent() *0.5f;
    morph->setOrigin(localCenter - childExtent *0.5f);
}

} // End of namespace Morphic
} // End of namespace Woden
