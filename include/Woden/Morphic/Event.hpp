#ifndef WODEN_MORPHIC_EVENT_HPP
#define WODEN_MORPHIC_EVENT_HPP

#include "Woden/Math/Vector2.hpp"
#include <memory>

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class Morph> MorphPtr;

typedef std::shared_ptr<class MorphicEvent>         MorphicEventPtr;
typedef std::shared_ptr<class MouseEvent>           MouseEventPtr;
typedef std::shared_ptr<class MouseButtonEvent>     MouseButtonEventPtr;
typedef std::shared_ptr<class MouseButtonDownEvent> MouseButtonDownEventPtr;
typedef std::shared_ptr<class MouseButtonUpEvent>   MouseButtonUpEventPtr;
typedef std::shared_ptr<class MouseMotionEvent>     MouseMotionEventPtr;
typedef std::shared_ptr<class MouseWheelEvent>      MouseWheelEventPtr;

class MorphicEvent : public std::enable_shared_from_this<MorphicEvent>
{
public:
    virtual void sentTo(const MorphPtr &morph);
};

class MouseEvent : public MorphicEvent
{
public:
    Math::Vector2 position;
};

class MouseButtonEvent : public MouseEvent
{
public:
    uint8_t buttonIndex = 0;
};

class MouseButtonDownEvent : public MouseButtonEvent
{
public:
    virtual void sentTo(const MorphPtr &morph) override;
};

class MouseButtonUpEvent : public MouseButtonEvent
{
public:
    virtual void sentTo(const MorphPtr &morph) override;
};

class MouseMotionEvent : public MouseEvent
{
public:
    virtual void sentTo(const MorphPtr &morph) override;
    
    uint32_t buttonState = 0;
    Math::Vector2 delta;
};

class MouseWheelEvent : public MouseEvent
{
public:
    virtual void sentTo(const MorphPtr &morph) override;
};

}
}

#endif //WODEN_MORPHIC_EVENT_HPP