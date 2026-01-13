#ifndef WODEN_MORPHIC_EVENT_HPP
#define WODEN_MORPHIC_EVENT_HPP

#include "Woden/Math/Vector2.hpp"

#include "SDL_keycode.h"
#include <memory>

namespace Woden
{
namespace GameFramework
{
typedef std::shared_ptr<class Actor> ActorPtr;
}

namespace Morphic
{
typedef std::shared_ptr<class Morph> MorphPtr;
}

namespace Events
{
typedef std::shared_ptr<class Event>                EventPtr;
typedef std::shared_ptr<class KeyboardEvent>        KeyboardEventPtr;
typedef std::shared_ptr<class KeyboardDownEvent>    KeyboardDownEventPtr;
typedef std::shared_ptr<class KeyboardUpEvent>      KeyboardUpEventPtr;
typedef std::shared_ptr<class MouseEvent>           MouseEventPtr;
typedef std::shared_ptr<class MouseEvent>           MouseEventPtr;
typedef std::shared_ptr<class MouseButtonEvent>     MouseButtonEventPtr;
typedef std::shared_ptr<class MouseButtonDownEvent> MouseButtonDownEventPtr;
typedef std::shared_ptr<class MouseButtonUpEvent>   MouseButtonUpEventPtr;
typedef std::shared_ptr<class MouseMotionEvent>     MouseMotionEventPtr;
typedef std::shared_ptr<class MouseWheelEvent>      MouseWheelEventPtr;
typedef std::shared_ptr<class TextInputEvent>       TextInputEventPtr;

using GameFramework::ActorPtr;
using Morphic::MorphPtr;

class Event : public std::enable_shared_from_this<Event>
{
public:
    virtual void sentToActor(const ActorPtr &actor);
    virtual void sentToMorph(const MorphPtr &morph);

    bool wasHandled = false;
};

class KeyboardEvent : public Event
{
public:
    bool isKeyRepeat = false;
    SDL_Keycode keySymbol;
};

class KeyboardDownEvent : public KeyboardEvent
{
public:
    virtual void sentToActor(const ActorPtr &actor) override;
    virtual void sentToMorph(const MorphPtr &morph) override;
};

class KeyboardUpEvent : public KeyboardEvent
{
public:
    virtual void sentToActor(const ActorPtr &actor) override;
    virtual void sentToMorph(const MorphPtr &morph) override;
};

class MouseEvent : public Event
{
public:

    template<typename FT>
    void withTranslationInverseDo(const Math::Vector2 &translation, FT&& aBlock)
    {
        auto oldPosition = position;
        position = position - translation;

        aBlock();

        position = oldPosition;
    }

    Math::Vector2 position;
};

class MouseButtonEvent : public MouseEvent
{
public:

    bool isLeftButton() const
    {
        return buttonIndex == 1;
    }

    bool isMiddleButton() const
    {
        return buttonIndex == 2;
    }

    bool isRightButton() const
    {
        return buttonIndex == 3;
    }


    uint8_t buttonIndex = 0;
};

class MouseButtonDownEvent : public MouseButtonEvent
{
public:
    virtual void sentToActor(const ActorPtr &actor) override;
    virtual void sentToMorph(const MorphPtr &morph) override;
};

class MouseButtonUpEvent : public MouseButtonEvent
{
public:
    virtual void sentToActor(const ActorPtr &actor) override;
    virtual void sentToMorph(const MorphPtr &morph) override;
};

class MouseMotionEvent : public MouseEvent
{
public:
    virtual void sentToActor(const ActorPtr &actor) override;
    virtual void sentToMorph(const MorphPtr &morph) override;

    bool hasLeftButtonDown() const
    {
        return (buttonState & 1) != 0;
    }

    bool hasMiddleButtonDown() const
    {
        return (buttonState & 2) != 0;
    }

    bool hasRightButtonDown() const
    {
        return (buttonState & 4) != 0;
    }

    uint32_t buttonState = 0;
    Math::Vector2 delta;
};

class MouseWheelEvent : public MouseEvent
{
public:
    virtual void sentToActor(const ActorPtr &actor) override;
    virtual void sentToMorph(const MorphPtr &morph) override;

    Math::Vector2 scrollAmount;
};

class TextInputEvent : public Event
{
public:
    std::string text;

    virtual void sentToActor(const ActorPtr &actor) override;
    virtual void sentToMorph(const MorphPtr &morph) override;
};

}
}

#endif //WODEN_MORPHIC_EVENT_HPP