#include "Woden/Morphic/Event.hpp"
#include "Woden/Morphic/Morph.hpp"

namespace Woden
{
namespace Morphic
{

void MorphicEvent::sentTo(const MorphPtr &morph)
{
    morph->handleUnknownEvent(shared_from_this());
}

void MouseButtonDownEvent::sentTo(const MorphPtr &morph)
{
    morph->handleMouseButtonDownEvent(std::static_pointer_cast<MouseButtonDownEvent> (shared_from_this()));
}

void MouseButtonUpEvent::sentTo(const MorphPtr &morph)
{
    morph->handleMouseButtonUpEvent(std::static_pointer_cast<MouseButtonUpEvent> (shared_from_this()));
}

void MouseMotionEvent::sentTo(const MorphPtr &morph)
{
    morph->handleMouseMotionEvent(std::static_pointer_cast<MouseMotionEvent> (shared_from_this()));
}

void MouseWheelEvent::sentTo(const MorphPtr &morph)
{
    morph->handleMouseWheelEvent(std::static_pointer_cast<MouseWheelEvent> (shared_from_this()));
}

}
}