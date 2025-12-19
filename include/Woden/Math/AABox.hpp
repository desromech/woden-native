#ifndef WODEN_MATH_AABOX_HPP
#define WODEN_MATH_AABOX_HPP

#include "Ray3D.hpp"

namespace Woden
{
namespace Math
{
/**
 * I am an axis aligned bounding box.
 */
class AABox
{
public:
    AABox()
        : minCorner(Vector3::PositiveInfinity()), maxCorner(Vector3::NegativeInfinity())
    {
    }

    AABox(const Vector3 &cminCorner, const Vector3 &cmaxCorner)
        : minCorner(cminCorner), maxCorner(cmaxCorner)
    {
    }

    bool isEmpty() const
    {
        return minCorner.x > maxCorner.x;
    }

    void insertPoint(const Vector3 &point)
    {
        minCorner = min(minCorner, point);
        maxCorner = max(maxCorner, point);
    }

    Vector3 minCorner;
    Vector3 maxCorner;
};

}
}

#endif //WODEN_MATH_AABOX_HPP