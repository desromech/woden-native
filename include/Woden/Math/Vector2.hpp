#ifndef WODEN_MATH_VECTOR2_HPP
#define WODEN_MATH_VECTOR2_HPP

#include "Scalar.hpp"
#include <ostream>

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

    Scalar dot(const Vector2 &o) const
    {
        return x*o.x + y*o.y;
    }

    Scalar cross(const Vector2 &o) const
    {
        return x*o.y - y*o.x;
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

    friend std::ostream &operator<<(std::ostream &out, const Vector2 &vector)
    {
        out << "Vector2(" << vector.x << ", " << vector.y << ")";
        return out;
    }

    Scalar x, y;
};

}    
}

#endif // WODEN_MATH_VECTOR2_HPP
