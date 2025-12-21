#ifndef WODEN_RENDERING_GUI_ELEMENT_HPP
#define WODEN_RENDERING_GUI_ELEMENT_HPP

#include "Woden/Math/Vector2.hpp"
#include "Woden/Math/Vector4.hpp"
#include <stdint.h>

namespace Woden
{
namespace Rendering
{

enum class GuiElementType : uint32_t
{
    SolidRectangle = 0,
    HorizontalGradient,
    VerticalGradient,
    TextCharacter,
    TexturedRectangle,
};

/**
 * I am a single GUI rectangle.
 */
struct GuiElement
{
    GuiElementType type;
    int padding;
    float borderRoundRadius;
    float borderSize;

    Math::Vector2 rectangleMin;
    Math::Vector2 rectangleMax;
    Math::Vector2 sourceImageRectangleMin;
    Math::Vector2 sourceImageRectangleMax;

    Math::Vector4 firstColor;
    Math::Vector4 secondColor;
    Math::Vector4 borderColor;
};

struct GUIPushConstants
{
    uint32_t hasTopLeftNDCOrigin;
    uint32_t padding;
    Math::Vector2 framebufferReciprocalExtent;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_GUI_ELEMENT_HPP