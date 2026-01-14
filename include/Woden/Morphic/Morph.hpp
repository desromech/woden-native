#ifndef WODEN_MORPHIC_MORPH_HPP
#define WODEN_MORPHIC_MORPH_HPP

#include "Woden/Events/Events.hpp"
#include "Woden/Math/Rectangle.hpp"
#include "Woden/Math/Vector4.hpp"
#include <assert.h>
#include <memory>
#include <vector>

namespace Woden
{
namespace Rendering
{
typedef std::shared_ptr<class GUIRenderer> GUIRendererPtr;
}

namespace Morphic
{
using namespace Woden::Math;
using namespace Woden::Events;

typedef std::shared_ptr<class Morph> MorphPtr;
typedef std::weak_ptr<class Morph> MorphWeakPtr;
typedef std::shared_ptr<class MorphicLayout> MorphicLayoutPtr;
typedef std::shared_ptr<class SystemWindow> SystemWindowPtr;

/**
 * I am morph
 */
class Morph : public std::enable_shared_from_this<Morph>
{
public:
    Morph();
    virtual ~Morph();
    
    static int runMainLoop();
    SystemWindowPtr openInSystemWindow();

    Vector2 getOrigin() const
    {
        return bounds.minCorner;
    }

    void setOrigin(const Vector2 &newOrigin)
    {
        bounds = Rectangle(newOrigin, newOrigin + bounds.extent());
    }

    Vector2 getExtent() const
    {
        return bounds.extent();
    }

    void setExtent(const Vector2 &newExtent)
    {
        bounds = Rectangle(bounds.minCorner, newExtent);
        updateLayout();
    }

    void setBounds(const Rectangle &newBounds)
    {
        bounds = newBounds;
        updateLayout();
    }

    Rectangle getLocalBounds() const
    {
        return Rectangle(Vector2(0), getExtent());
    }

    void addSubmorph(const MorphPtr &morph)
    {
        assert(!morph->owner.lock());
        morph->owner = shared_from_this();
        submorphs.push_back(morph);
    }

    virtual void fullUpdate(Math::Scalar deltaTime);
    virtual void updateChildren(Math::Scalar deltaTime);
    virtual void update(Math::Scalar deltaTime);

    virtual void fullDrawWith(const Rendering::GUIRendererPtr &renderer);
    virtual void drawWith(const Rendering::GUIRendererPtr &renderer);
    virtual void drawChildrenWith(const Rendering::GUIRendererPtr &renderer);

    virtual void processEvent(const EventPtr &event);

    virtual void handleUnknownEvent(const EventPtr &event);

    virtual void handleKeyboardDownEvent(const KeyboardDownEventPtr &event);
    virtual void handleKeyboardUpEvent(const KeyboardUpEventPtr &event);
    virtual void handleTextInputEvent(const TextInputEventPtr &event);

    virtual void handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event);
    virtual void handleMouseButtonUpEvent(const MouseButtonUpEventPtr &event);
    virtual void handleMouseMotionEvent(const MouseMotionEventPtr &event);
    virtual void handleMouseWheelEvent(const MouseWheelEventPtr &event);

    virtual void gotKeyboardFocus();
    virtual void lostKeyboardFocus();

    virtual void takeKeyboardFocus();
    virtual void setNewKeyboardFocus(const MorphPtr &newKeyboardFocus);

    const MorphicLayoutPtr &getLayout() const
    {
        return layout;
    }

    void setLayout(const MorphicLayoutPtr &newLayout)
    {
        layout = newLayout;
        updateLayout();
    }

    virtual void updateLayout();

    MorphWeakPtr owner;
    Rectangle bounds = Rectangle(Vector2(0, 0), Vector2(50, 40));
    Vector4 color = Vector4(0, 0, 1, 1);
    std::vector<MorphPtr> submorphs;
    
protected:
    MorphicLayoutPtr layout;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_MORPH_HPP