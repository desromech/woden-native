#ifndef WODEN_MORPHIC_MORPH_HPP
#define WODEN_MORPHIC_MORPH_HPP

#include "Event.hpp"
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

typedef std::shared_ptr<class Morph> MorphPtr;
typedef std::weak_ptr<class Morph> MorphWeakPtr;
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

    virtual void processEvent(const MorphicEventPtr &event);

    virtual void handleUnknownEvent(const MorphicEventPtr &event);
    virtual void handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event);
    virtual void handleMouseButtonUpEvent(const MouseButtonUpEventPtr &event);
    virtual void handleMouseMotionEvent(const MouseMotionEventPtr &event);
    virtual void handleMouseWheelEvent(const MouseWheelEventPtr &event);

    MorphWeakPtr owner;
    Rectangle bounds = Rectangle(Vector2(0, 0), Vector2(50, 40));
    Vector4 color = Vector4(0, 0, 1, 1);
    std::vector<MorphPtr> submorphs;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_MORPH_HPP