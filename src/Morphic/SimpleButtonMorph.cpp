#include "Woden/Morphic/SimpleButtonMorph.hpp"

namespace Woden
{
namespace Morphic
{

SimpleButtonMorph::SimpleButtonMorph()
{
    color = Vector4(0.7f, 0.7f, 0.7f, 1.0f);

}

Vector4 SimpleButtonMorph::getCurrentColor() const
{
    if(hasButtonDown) return clickColor;
    if(hasMouseFocus()) return focusColor;
    return color;
}

void SimpleButtonMorph::setLabel(const std::string &labelText)
{
    if(label)
    {
        label->deleteMorph();
        label.reset();
        label.reset();
    }

    label = std::make_shared<StringMorph> ();
    label->label = labelText;
    addSubmorph(label);
}

void SimpleButtonMorph::handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    if(event->isLeftButton())
    {
        hasButtonDown = true;
        event->wasHandled = true;
    }
}

void SimpleButtonMorph::handleMouseButtonUpEvent(const MouseButtonUpEventPtr &event)
{
    if(event->isLeftButton())
    {
        hasButtonDown = false;
        event->wasHandled = true;
    }

}

void SimpleButtonMorph::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    takeMouseFocus();
    event->wasHandled = true;
}

} // End of namespace Morphic
} // End of namespace Woden
    