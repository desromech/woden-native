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

Math::Vector2 PackingLayoutMorphElement::getFixedExtent()
{
    return morph->getExtent();
}

void PackingLayoutMorphElement::applyRectangle(const Math::Rectangle &rectangle)
{
    morph->setBounds(rectangle);
}

void HorizontalPackingLayout::applyWithLocalBounds(const Math::Rectangle &localBounds)
{
    // First pass: gather the proportion and fixed height
    int totalProportion = 0;
    Scalar fixedWidth = 0;

    for(auto &element : elements)
    {
        if(element->proportion != 0)
            totalProportion += element->proportion;
        else
            fixedWidth += element->getFixedExtent().x;
    }

    auto localBoundsExtent = localBounds.extent();
    Scalar remainingWidth = std::max(localBoundsExtent.x - fixedWidth, Scalar(0));

    // Second pass: per-element computation.
    auto destinationX = localBounds.minCorner.x;
    auto height = localBounds.extent().y;

    for(auto &element : elements)
    {
        if(element->proportion == 0)
        {
            auto origin = Vector2(destinationX, localBounds.minCorner.y);
            auto fixedExtent = element->getFixedExtent();
            if(element->expanded)
            {
                // Expanded
                element->applyRectangle(Rectangle::WithOriginAndExtent(origin, Vector2(fixedExtent.x, height)).floorRounded());
            }
            else
            {
                // Non-expanded
                element->applyRectangle(Rectangle::WithOriginAndExtent(origin, fixedExtent).floorRounded());

            }
            destinationX += fixedExtent.x;
        }
        else
        {
            Scalar elementWidth = remainingWidth * element->proportion / totalProportion;
            element->applyRectangle(Rectangle::WithOriginAndExtent(
                Vector2(destinationX, localBounds.minCorner.y),
                Vector2(elementWidth, height)).floorRounded());

            destinationX += elementWidth;
        }
    }
}


void VerticalPackingLayout::applyWithLocalBounds(const Math::Rectangle &localBounds)
{
    // First pass: gather the proportion and fixed height
    int totalProportion = 0;
    Scalar fixedHeight = 0;

    for(auto &element : elements)
    {
        if(element->proportion != 0)
            totalProportion += element->proportion;
        else
            fixedHeight += element->getFixedExtent().y;
    }

    auto localBoundsExtent = localBounds.extent();
    Scalar remainingHeight = std::max(localBoundsExtent.y - fixedHeight, Scalar(0));

    // Second pass: per-element computation.
    auto destinationY = localBounds.minCorner.y;
    auto width = localBounds.extent().x;

    for(auto &element : elements)
    {
        if(element->proportion == 0)
        {
            auto origin = Vector2(localBounds.minCorner.x, destinationY);
            auto fixedExtent = element->getFixedExtent();
            if(element->expanded)
            {
                // Expanded
                element->applyRectangle(Rectangle::WithOriginAndExtent(origin, Vector2(width, fixedExtent.y)).floorRounded());
            }
            else
            {
                // Non-expanded
                element->applyRectangle(Rectangle::WithOriginAndExtent(origin, fixedExtent).floorRounded());

            }
            destinationY += fixedExtent.y;
        }
        else
        {
            Scalar elementHeight = remainingHeight * element->proportion / totalProportion;
            element->applyRectangle(Rectangle::WithOriginAndExtent(
                Vector2(localBounds.minCorner.x, destinationY),
                Vector2(width, elementHeight)).floorRounded());

            destinationY += elementHeight;
        }
    }
}

} // End of namespace Morphic
} // End of namespace Woden
