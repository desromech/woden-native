#ifndef WODEN_MATH_VECTOR2_HPP
#define WODEN_MATH_VECTOR2_HPP

#include "Scalar.hpp"

namespace Woden
{
namespace Math
{
/**
 * 2D vector.
 */
class Vector2
{
public:
    Vector2(Scalar s = 0) : x(s), y(s)
    {
    }

    Vector2(Scalar cx, Scalar cy) : x(cx), y(cy)
    {
    }

    Vector2 abs() const
    {
        return Vector2(Woden::Math::abs(x), Woden::Math::abs(y));
    }

    Vector2 operator+(const Vector2 &o) const
    {
        return Vector2(x + o.x, y + o.y);
    }

    Vector2 operator-(const Vector2 &o) const
    {
        return Vector2(x - o.x, y - o.y);
    }

    Vector2 operator*(const Vector2 &o) const
    {
        return Vector2(x * o.x, y * o.y);
    }

    Vector2 operator/(const Vector2 &o) const
    {
        return Vector2(x / o.x, y / o.y);
    }

    Scalar x, y;
};

}    
}

#endif // WODEN_MATH_VECTOR2_HPP
