#ifndef WODEN_MATH_VECTOR4_HPP
#define WODEN_MATH_VECTOR4_HPP

#include "Scalar.hpp"

namespace Woden
{
namespace Math
{
/**
 * 4D vector.
 */
class Vector4
{
public:
    Vector4(Scalar s = 0) : x(s), y(s), z(s), w(s)
    {
    }

    Vector4(Scalar cx, Scalar cy, Scalar cz, Scalar cw) : x(cx), y(cy), z(cz), w(cw)
    {
    }

    Vector4 abs() const
    {
        return Vector4(Woden::Math::abs(x), Woden::Math::abs(y), Woden::Math::abs(z), Woden::Math::abs(w));
    }

    Scalar dot(const Vector4 &o) const
    {
        return x*o.x + y*o.y + z*o.z + w*o.w;
    }

    Vector4 operator+(const Vector4 &o) const
    {
        return Vector4(x + o.x, y + o.y, z + o.z, w + o.w);
    }

    Vector4 operator-(const Vector4 &o) const
    {
        return Vector4(x - o.x, y - o.y, z - o.z, w - o.w);
    }

    Vector4 operator*(const Vector4 &o) const
    {
        return Vector4(x * o.x, y * o.y, z * o.z, w * o.w);
    }

    Vector4 operator/(const Vector4 &o) const
    {
        return Vector4(x / o.x, y / o.y, z / o.z, w / o.w);
    }

    Scalar x, y, z, w;
};

}    
}

#endif // WODEN_MATH_VECTOR4_HPP
