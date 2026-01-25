#ifndef WODEN_MORPHIC_SIMPLE_BUTTON_MORPH_HPP
#define WODEN_MORPHIC_SIMPLE_BUTTON_MORPH_HPP

#include "BorderedMorph.hpp"
#include "StringMorph.hpp"
#include <functional>

namespace Woden
{
namespace Morphic
{

class SimpleButtonMorph : public BorderedMorph
{
public:
    SimpleButtonMorph();

    virtual Vector4 getCurrentColor() const override;

    void setLabel(const std::string &labelText);

    virtual void onMouseButtonDownEvent(const MouseButtonDownEventPtr &event);
    virtual void onMouseButtonUpEvent(const MouseButtonUpEventPtr &event);
    virtual void onMouseMotionEvent(const MouseMotionEventPtr &event);

    Vector4 focusColor = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
    Vector4 clickColor = Vector4(0.9f, 0.9f, 0.9f, 1.0f);
    bool hasButtonDown = false;
    StringMorphPtr label;

    std::function<void (MorphPtr)> onClickAction;

};

} // End of namespace Morphic
} // End of namespace Woden

#endif // WODEN_MORPHIC_SIMPLE_BUTTON_MORPH_HPP