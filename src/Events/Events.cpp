#include "Woden/Events/Events.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/Morphic/Morph.hpp"

namespace Woden
{
namespace Events
{
using namespace Woden::Morphic;

// Event
void Event::sentToActor(const ActorPtr &actor)
{
    actor->handleUnknownEvent(shared_from_this());
}

void Event::sentToMorph(const MorphPtr &morph)
{
    morph->handleUnknownEvent(shared_from_this());
}

// Keyboard down event.
void KeyboardDownEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleKeyboardDownEvent(std::static_pointer_cast<KeyboardDownEvent> (shared_from_this()));
}
void KeyboardDownEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleKeyboardDownEvent(std::static_pointer_cast<KeyboardDownEvent> (shared_from_this()));
}

// Keyboard up event.
void KeyboardUpEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleKeyboardUpEvent(std::static_pointer_cast<KeyboardUpEvent> (shared_from_this()));
}
void KeyboardUpEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleKeyboardUpEvent(std::static_pointer_cast<KeyboardUpEvent> (shared_from_this()));
}

// Mouse button down event
void MouseButtonDownEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleMouseButtonDownEvent(std::static_pointer_cast<MouseButtonDownEvent> (shared_from_this()));
}

void MouseButtonDownEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleMouseButtonDownEvent(std::static_pointer_cast<MouseButtonDownEvent> (shared_from_this()));
}

// Mouse button up event
void MouseButtonUpEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleMouseButtonUpEvent(std::static_pointer_cast<MouseButtonUpEvent> (shared_from_this()));
}

void MouseButtonUpEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleMouseButtonUpEvent(std::static_pointer_cast<MouseButtonUpEvent> (shared_from_this()));
}

// Mouse motion event
void MouseMotionEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleMouseMotionEvent(std::static_pointer_cast<MouseMotionEvent> (shared_from_this()));
}

void MouseMotionEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleMouseMotionEvent(std::static_pointer_cast<MouseMotionEvent> (shared_from_this()));
}

// Mouse wheel event.
void MouseWheelEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleMouseWheelEvent(std::static_pointer_cast<MouseWheelEvent> (shared_from_this()));
}

void MouseWheelEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleMouseWheelEvent(std::static_pointer_cast<MouseWheelEvent> (shared_from_this()));
}

// Text input event
void TextInputEvent::sentToActor(const ActorPtr &actor)
{
    actor->handleTextInputEvent(std::static_pointer_cast<TextInputEvent> (shared_from_this()));
}
void TextInputEvent::sentToMorph(const MorphPtr &morph)
{
    morph->handleTextInputEvent(std::static_pointer_cast<TextInputEvent> (shared_from_this()));
}

}
}