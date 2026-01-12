#include "Woden/Events/Events.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/Morphic/Morph.hpp"

namespace Woden
{
namespace Events
{
using namespace Woden::Morphic;

void Event::sentToActor(const ActorPtr &actor)
{
    actor->handleUnknownEvent(shared_from_this());
}

void Event::sentToMorph(const MorphPtr &morph)
{
    morph->handleUnknownEvent(shared_from_this());
}

void MouseButtonDownEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleMouseButtonDownEvent(std::static_pointer_cast<MouseButtonDownEvent> (shared_from_this()));
}

void MouseButtonDownEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleMouseButtonDownEvent(std::static_pointer_cast<MouseButtonDownEvent> (shared_from_this()));
}

void MouseButtonUpEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleMouseButtonUpEvent(std::static_pointer_cast<MouseButtonUpEvent> (shared_from_this()));
}

void MouseButtonUpEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleMouseButtonUpEvent(std::static_pointer_cast<MouseButtonUpEvent> (shared_from_this()));
}

void MouseMotionEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleMouseMotionEvent(std::static_pointer_cast<MouseMotionEvent> (shared_from_this()));
}

void MouseMotionEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleMouseMotionEvent(std::static_pointer_cast<MouseMotionEvent> (shared_from_this()));
}

void MouseWheelEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleMouseWheelEvent(std::static_pointer_cast<MouseWheelEvent> (shared_from_this()));
}

void MouseWheelEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleMouseWheelEvent(std::static_pointer_cast<MouseWheelEvent> (shared_from_this()));
}

}
}