#ifndef WODEN_MORPHIC_LAYOUT_HPP
#define WODEN_MORPHIC_LAYOUT_HPP

#include "Woden/Math/Rectangle.hpp"
#include <memory>
#include <vector>

namespace Woden
{
namespace Morphic
{

typedef std::shared_ptr<class MorphicLayout> MorphicLayoutPtr;
typedef std::shared_ptr<class Morph> MorphPtr;
typedef std::shared_ptr<class PackingLayoutElement> PackingLayoutElementPtr;

/**
 * Morphic layout abstract base class.
 */
class MorphicLayout
{
public:
    virtual void applyWithLocalBounds(const Math::Rectangle &localBounds) = 0;
};

/**
 * I am a layout that fills the parent morph with a single morph
 */
class FillMorphicLayout : public MorphicLayout
{
public:
    virtual void applyWithLocalBounds(const Math::Rectangle &localBounds);

    MorphPtr morph;
};

/**
 * I am a layout that centers a single morph with its parent
 */
class CenterMorphicLayout : public MorphicLayout
{
public:
    virtual void applyWithLocalBounds(const Math::Rectangle &localBounds);

    MorphPtr morph;
};

/**
 * I am an element in a packed layout
 */
class PackingLayoutElement
{
public:
    int proportion = 0;
    bool expanded = false;

    virtual Math::Vector2 getFixedExtent()
    {
        return Math::Vector2(0, 0);
    }

    virtual void applyRectangle(const Math::Rectangle &rectangle)
    {
        (void)rectangle;
    }
};

/**
 * I am a morph element in a packed layout
 */
class PackingLayoutMorphElement : public PackingLayoutElement
{
public:
    virtual Math::Vector2 getFixedExtent() override;
    virtual void applyRectangle(const Math::Rectangle &rectangle) override;

    MorphPtr morph;
};

/**
 * I am a layout inside a packing layout
 */
class PackingLayoutLayoutElement : public PackingLayoutElement
{
public:
    virtual Math::Vector2 getFixedExtent() override;
    virtual void applyRectangle(const Math::Rectangle &rectangle) override;

    MorphicLayoutPtr layout;
};

/**
 * I am a layout that is used for packing a set of morphs in a vertical or horizontal layout
 */
class PackingLayout : public MorphicLayout
{
public:
    void addMorph(const MorphPtr &morph, int proportion = 1, bool expanded = false)
    {
        auto element = std::make_shared<PackingLayoutMorphElement> ();
        element->morph = morph;
        element->proportion = proportion;
        element->expanded = expanded;
        elements.push_back(element);
    }

    void addLayout(const MorphicLayoutPtr &layout, int proportion = 1, bool expanded = false)
    {
        auto element = std::make_shared<PackingLayoutLayoutElement> ();
        element->layout = layout;
        element->proportion = proportion;
        element->expanded = expanded;
        elements.push_back(element);
    }

    std::vector<PackingLayoutElementPtr> elements;
};

/**
 * I am a layout that is used for packing a set of morphs in a horizontal layout
 */
class HorizontalPackingLayout : public PackingLayout
{
public:
    virtual void applyWithLocalBounds(const Math::Rectangle &localBounds);
};


/**
 * I am a layout that is used for packing a set of morphs in a vertical layout
 */
class VerticalPackingLayout : public PackingLayout
{
public:
    virtual void applyWithLocalBounds(const Math::Rectangle &localBounds);
};


} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_LAYOUT_HPP