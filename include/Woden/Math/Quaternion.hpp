#ifndef WODEN_MATH_QUATERNION_HPP
#define WODEN_MATH_QUATERNION_HPP

#include "Scalar.hpp"
#include "Matrix3x3.hpp"

namespace Woden
{
namespace Math
{

/**
 * I am a quaternion
 */
struct Quaternion
{
    Quaternion(Scalar cw = 0)
        : x(0), y(0), z(0), w(cw)
    {
    }

    Quaternion(Scalar cx, Scalar cy, Scalar cz, Scalar cw)
        : x(cx), y(cy), z(cz), w(cw)
    {
    }

    static Quaternion Identity()
    {
        return Quaternion(0, 0, 0, 1);
    }
    
    static Quaternion XRotation(Scalar angle)
    {
        auto c = cos(angle*0.5);
        auto s = sin(angle*0.5);
        return Quaternion(s, 0, 0, c);
    }

    static Quaternion YRotation(Scalar angle)
    {
        auto c = cos(angle*0.5);
        auto s = sin(angle*0.5);
        return Quaternion(0, s, 0, c);
    }

    static Quaternion ZRotation(Scalar angle)
    {
        auto c = cos(angle*0.5);
        auto s = sin(angle*0.5);
        return Quaternion(0, 0, s, c);
    }

    Quaternion conjugated() const
    {
        return Quaternion(-x, -y, -z, -w);
    }

    Scalar dot(const Quaternion &o) const
    {
        return x*o.x + y*o.y + z*o.z + w*o.w;
    }

    Scalar length2() const
    {
        return dot(*this);
    }

    Scalar length() const
    {
        return sqrt(length2());
    }

    Matrix3x3 asMatrix() const
    {
        return Matrix3x3(
            1.0 - (2.0*j*j) - (2.0*k*k),
            (2.0*i*j) - (2.0*k*r),
            (2.0*i*k) + (2.0*j*r),

            (2.0*i*j) + (2.0*k*r),
            1.0 - (2.0*i*i) - (2.0*k*k),
            (2.0*j*k) - (2.0*i*r),

            (2.0*i*k) - (2.0*j*r),
            (2.0*j*k) + (2.0*i*r),
            1.0 - (2.0*i*i) - (2.0*j*j)
        );
    }

    bool operator==(const Quaternion &o) const
    {
        return x == o.x && y == o.y && z == o.z && w == o.w;
    }

    bool operator!=(const Quaternion &o) const
    {
        return !(*this == o);
    }

    Quaternion operator+(const Quaternion &o) const
    {
        return Quaternion(x + o.x, y + o.y, z + o.z, w + o.w);
    }

    Quaternion operator-(const Quaternion &o) const
    {
        return Quaternion(x - o.x, y - o.y, z - o.z, w - o.w);
    }

    Quaternion operator*(const Quaternion &o)
    {
        return Quaternion(
            (r * o.i) + (i * o.r) + (j * o.k) - (k * o.j),
            (r * o.j) - (i * o.k) + (j * o.r) + (k * o.i),
            (r * o.k) + (i * o.j) - (j * o.i) + (k * o.r),    
            (r * o.r) - (i * o.i) - (j * o.j) - (k * o.k)
        );
    }

    union
    {
        struct
        {
            Scalar x, y, z, w;
        };
        struct
        {
            Scalar i, j, k, r;
        };
    };
};
} // End of namespace Math
} // End of namespace Woden

#endif //WODEN_MATH_QUATERNION_HPP