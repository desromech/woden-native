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

    Scalar length2() const
    {
        return dot(*this);
    }

    Scalar length() const
    {
        return sqrt(length2());
    }

    Vector2 normalized() const
    {
        auto l = length();
        if(l <= 0)
            return Vector2(0);
        return Vector2(x / l, y / l);
    }
    
    bool operator==(const Vector2 &o) const
    {
        return x == o.x && y == o.y;
    }

    bool operator!=(const Vector2 &o) const
    {
        return !(*this == o);
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

inline bool closeTo(const Vector2 &a, const Vector2 &b)
{
    return closeTo(a.x, b.x) && closeTo(a.y, b.y);
}

}    
}

#endif // WODEN_MATH_VECTOR2_HPP
