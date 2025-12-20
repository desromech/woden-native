#ifndef WODEN_MORPHIC_MORPH_HPP
#define WODEN_MORPHIC_MORPH_HPP

#include "Woden/Math/Rectangle.hpp"
#include "Woden/Math/Vector4.hpp"
#include <assert.h>
#include <memory>
#include <vector>

namespace Woden
{
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

    void addSubmorph(const MorphPtr &morph)
    {
        assert(!morph->owner.lock());
        morph->owner = shared_from_this();
        submorphs.push_back(morph);
    }

    MorphWeakPtr owner;
    Rectangle bounds = Rectangle(Vector2(0, 0), Vector2(50, 40));
    Vector4 color = Vector4(0, 0, 1, 1);
    std::vector<MorphPtr> submorphs;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_MORPH_HPP