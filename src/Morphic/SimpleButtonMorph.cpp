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

    label = MakeMorph<StringMorph> ();
    label->label = labelText;
    label->fitSize();
    addMorph(label);

    setExtent(label->getExtent() + Vector2(20, 20));

    auto layout = std::make_shared<CenterMorphicLayout> ();
    layout->morph = label;
    setLayout(layout);
}

void SimpleButtonMorph::onMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    if(event->isLeftButton())
    {
        hasButtonDown = true;
        event->wasHandled = true;
    }
}

void SimpleButtonMorph::onMouseButtonUpEvent(const MouseButtonUpEventPtr &event)
{
    if(event->isLeftButton())
    {
        hasButtonDown = false;
        event->wasHandled = true;

        if(onClickAction)
            onClickAction(shared_from_this());
    }

}

void SimpleButtonMorph::onMouseMotionEvent(const MouseMotionEventPtr &event)
{
    takeMouseFocus();
    event->wasHandled = true;
}

} // End of namespace Morphic
} // End of namespace Woden
    