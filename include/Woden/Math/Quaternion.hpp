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
        auto c = cos(Scalar(angle*0.5));
        auto s = sin(Scalar(angle*0.5));
        return Quaternion(s, 0, 0, c);
    }

    static Quaternion YRotation(Scalar angle)
    {
        auto c = cos(Scalar(angle*0.5));
        auto s = sin(Scalar(angle*0.5));
        return Quaternion(0, s, 0, c);
    }

    static Quaternion ZRotation(Scalar angle)
    {
        auto c = cos(Scalar(angle*0.5));
        auto s = sin(Scalar(angle*0.5));
        return Quaternion(0, 0, s, c);
    }

    static Quaternion XRotationDegrees(Scalar angle)
    {
        return XRotation(Scalar(angle * M_PI / 180.0));
    }
    
    static Quaternion YRotationDegrees(Scalar angle)
    {
        return YRotation(Scalar(angle * M_PI / 180.0));
    }
    
    static Quaternion ZRotationDegrees(Scalar angle)
    {
        return ZRotation(Scalar(angle * M_PI / 180.0));
    }

    Quaternion conjugated() const
    {
        return Quaternion(-x, -y, -z, w);
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
            Scalar(1.0 - (2.0*j*j) - (2.0*k*k)),
            Scalar((2.0*i*j) - (2.0*k*r)),
            Scalar((2.0*i*k) + (2.0*j*r)),

            Scalar((2.0*i*j) + (2.0*k*r)),
            Scalar(1.0 - (2.0*i*i) - (2.0*k*k)),
            Scalar((2.0*j*k) - (2.0*i*r)),

            Scalar((2.0*i*k) - (2.0*j*r)),
            Scalar((2.0*j*k) + (2.0*i*r)),
            Scalar(1.0 - (2.0*i*i) - (2.0*j*j))
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

    Quaternion operator*(const Quaternion &o) const
    {
        return Quaternion(
            (r * o.i) + (i * o.r) + (j * o.k) - (k * o.j),
            (r * o.j) - (i * o.k) + (j * o.r) + (k * o.i),
            (r * o.k) + (i * o.j) - (j * o.i) + (k * o.r),    
            (r * o.r) - (i * o.i) - (j * o.j) - (k * o.k)
        );
    }

    Vector3 rotateVector(const Vector3 &v) const
    {
        return ((*this)*Quaternion(v.x, v.y, v.z, 0)*conjugated()).xyz();
    }

    Vector3 inverseRotateVector(const Vector3 &v) const
    {
        return (conjugated()*Quaternion(v.x, v.y, v.z, 0)*(*this)).xyz();
    }

    Vector3 xyz() const
    {
        return Vector3(x, y, z);
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