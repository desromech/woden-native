#ifndef WODEN_MATH_RECTANGLE_HPP
#define WODEN_MATH_RECTANGLE_HPP

#include "Vector2.hpp"

namespace Woden
{
namespace Math
{
/**
 * I am an axis aligned bounding box.
 */
class Rectangle
{
public:
    Rectangle()
        : minCorner(Vector2::PositiveInfinity()), maxCorner(Vector2::NegativeInfinity())
    {
    }

    Rectangle(const Vector2 &cminCorner, const Vector2 &cmaxCorner)
        : minCorner(cminCorner), maxCorner(cmaxCorner)
    {
    }

    static Rectangle WithHalfExtent(const Vector2 &halfExtent)
    {
        return Rectangle(-halfExtent, halfExtent);
    }

    static Rectangle WithOriginAndExtent(const Vector2 &origin, const Vector2 &extent)
    {
        return Rectangle(origin, origin + extent);
    }

    bool isEmpty() const
    {
        return minCorner.x > maxCorner.x;
    }

    void insertPoint(const Vector2 &point)
    {
        minCorner = min(minCorner, point);
        maxCorner = max(maxCorner, point);
    }

    bool containsPoint(const Vector2 &point)
    {
        return minCorner.x <= point.x && point.x <= maxCorner.x &&
               minCorner.y <= point.y && point.y <= maxCorner.y;
    }

    Vector2 extent() const
    {
        return maxCorner - minCorner;
    }

    Vector2 halfExtent() const
    {
        return (maxCorner - minCorner)*Vector2(0.5);
    }

    Vector2 center() const
    {
        return minCorner + halfExtent();
    }

    Rectangle floorRounded() const
    {
        return Rectangle(minCorner.floorRounded(), maxCorner.floorRounded());
    }

    Rectangle translatedBy(const Vector2 &translation) const
    {
        return Rectangle(minCorner + translation, maxCorner + translation);
    }

    Vector2 minCorner;
    Vector2 maxCorner;
};

}
}

#endif //WODEN_MATH_RECTANGLE_HPP
