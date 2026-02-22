#ifndef WODEN_MATH_PLANE_HPP
#define WODEN_MATH_PLANE_HPP

#include "Vector3.hpp"

namespace Woden
{
namespace Math
{

/**
 * I am a 3D plane in hessian form:
 * <P, N> = D
 */
struct Plane
{
    Plane()
    {}
    Plane(Vector3 cnormal, Scalar cdistance)
        : normal(cnormal), distance(cdistance)
    {}

    static Plane WithPoints(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
    {
        auto u = p2 - p1;
        auto v = p3 - p1;
        auto n = u.cross(v).normalized();
        auto d = p1.dot(n);
        return Plane(n, d);
    }

    Scalar signedDistanceToPoint(const Vector3 &p)
    {
        return normal.dot(p) - distance;
    }

    bool isPointInsideOrBehind(const Vector3 &p, Scalar epsilon)
    {
        return signedDistanceToPoint(p) < epsilon;
    }

    Vector3 normal = Vector3::Zeros();
    Scalar distance = 0;
};

} // End of namespace Math
} // End of namespace Woden

#endif //WODEN_MATH_PLANE_HPP
