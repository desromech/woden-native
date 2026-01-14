#include "Woden/Morphic/SimpleButtonMorph.hpp"
#include "Woden/Morphic/Layout.hpp"


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
        setLayout(nullptr);
    }

    label = std::make_shared<StringMorph> ();
    label->label = labelText;
    label->fitSize();
    addSubmorph(label);

    auto layout = std::make_shared<CenterMorphicLayout> ();
    layout->morph = label;
    setLayout(layout);
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

        if(onClickAction)
            onClickAction(shared_from_this());
    }

}

void SimpleButtonMorph::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    takeMouseFocus();
    event->wasHandled = true;
}

} // End of namespace Morphic
} // End of namespace Woden
    