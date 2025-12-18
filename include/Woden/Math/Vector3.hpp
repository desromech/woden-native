#ifndef WODEN_MATH_VECTOR3_HPP
#define WODEN_MATH_VECTOR3_HPP

#include "Scalar.hpp"

namespace Woden
{
namespace Math
{
/**
 * 3D vector.
 */
class Vector3
{
public:
    Vector3(Scalar s = 0) : x(s), y(s), z(s)
    {
    }

    Vector3(Scalar cx, Scalar cy, Scalar cz) : x(cx), y(cy), z(cz)
    {
    }

    Vector3 abs() const
    {
        return Vector3(Woden::Math::abs(x), Woden::Math::abs(y), Woden::Math::abs(z));
    }

    Scalar dot(const Vector3 &o) const
    {
        return x*o.x + y*o.y + z*o.z;
    }

    Vector3 operator+(const Vector3 &o) const
    {
        return Vector3(x + o.x, y + o.y, z + o.z);
    }

    Vector3 operator-(const Vector3 &o) const
    {
        return Vector3(x - o.x, y - o.y, z - o.z);
    }

    Vector3 operator*(const Vector3 &o) const
    {
        return Vector3(x * o.x, y * o.y, z * o.z);
    }

    Vector3 operator/(const Vector3 &o) const
    {
        return Vector3(x / o.x, y / o.y, z / o.z);
    }

    Scalar x, y, z;
};

}    
}

#endif // WODEN_MATH_VECTOR3_HPP
