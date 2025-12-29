#include "Woden/Morphic/Morph.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

namespace Woden
{
namespace Morphic
{

Morph::Morph()
{
}

Morph::~Morph()
{
}

void Morph::fullDrawWith(const Rendering::GUIRendererPtr &renderer)
{
    drawWith(renderer);
    drawChildrenWith(renderer);
}

void Morph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    renderer->fillRectangleWithColor(getLocalBounds(), color);
}

void Morph::drawChildrenWith(const Rendering::GUIRendererPtr &renderer)
{
    for(auto &submorph : submorphs)
        submorph->fullDrawWith(renderer);
}

void Morph::processEvent(const MorphicEventPtr &event)
{
    event->sentTo(shared_from_this());
}

void Morph::handleUnknownEvent(const MorphicEventPtr &event)
{
}

void Morph::handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    printf("Mouse button %d down event %f,%f\n",
        event->buttonIndex,
        event->position.x, event->position.y
    );
}

void Morph::handleMouseButtonUpEvent(const MouseButtonUpEventPtr &event)
{
    printf("Mouse button %d up event %f,%f\n",
        event->buttonIndex,
        event->position.x, event->position.y
    );
}

void Morph::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    printf("Mouse motion event %04x %f,%f %f,%f\n",
        event->buttonState,
        event->position.x, event->position.y,
        event->delta.x, event->delta.y
    );
}

void Morph::handleMouseWheelEvent(const MouseWheelEventPtr &event)
{
}

} // End of namespace Morphic
} // End of namespace Woden