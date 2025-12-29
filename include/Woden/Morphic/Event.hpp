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

    bool wasHandled = false;
};

class MouseEvent : public MorphicEvent
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
    virtual void sentTo(const MorphPtr &morph) override;

    Math::Vector2 scrollAmount;
};

}
}

#endif //WODEN_MORPHIC_EVENT_HPP