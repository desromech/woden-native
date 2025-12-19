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

    static AABox WithHalfExtent(const Vector3 &halfExtent)
    {
        return AABox(-halfExtent, halfExtent);
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

    RayCastingResult intersectionsWithRay(const Ray3D &ray)
    {
        // Slab testing algorithm from: A Ray-Box Intersection Algorithm andEfficient Dynamic Voxel Rendering. By Majercik et al
        auto t0 = (minCorner - ray.origin)*ray.inverseDirection;
        auto t1 = (maxCorner - ray.origin)*ray.inverseDirection;

        auto tmin = min(t0, t1);
        auto tmax = max(t0, t1);

        auto maxTMin = max(max(max(tmin.x, tmin.y), tmin.z), ray.tmin);
        auto minTMax = min(min(min(tmax.x, tmax.y), tmax.z), ray.tmax);

        RayCastingResult result;
        result.ray = ray;
        auto hasIntersection = maxTMin <= minTMax;
        if(hasIntersection)
        {
            result.intersectionPoints.reserve(2);
            result.intersectionPoints.push_back(min(maxTMin, minTMax));
            result.intersectionPoints.push_back(max(maxTMin, minTMax));
        }

        return result;
    }


    Vector3 minCorner;
    Vector3 maxCorner;
};

}
}

#endif //WODEN_MATH_AABOX_HPP
