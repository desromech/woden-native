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

    static AABox Empty()
    {
        return AABox();
    }


    static AABox WithHalfExtent(const Vector3 &halfExtent)
    {
        return AABox(-halfExtent, halfExtent);
    }

    template<typename C>
    static AABox Encompassing(const C &collection)
    {
        auto result = Empty();
        for(auto &point : collection)
            result.insertPoint(point);
        return result;
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

    Vector3 halfExtent() const
    {
        return (maxCorner - minCorner)*Vector3(0.5);
    }

    Vector3 center() const
    {
        return minCorner + halfExtent();
    }

    Vector3 computeNormalForPoint(const Vector3 &point) const
    {
        auto delta = point - center();
        auto deltaAbsolute = delta.abs() / halfExtent();
        if(deltaAbsolute.x >= deltaAbsolute.y)
        {
            if(deltaAbsolute.x >= deltaAbsolute.z)
                return Vector3(sign(delta.x), 0, 0);
            else
                return Vector3(0, 0, sign(delta.z));
        }
        else
        {
            if(deltaAbsolute.y >= deltaAbsolute.z)
                return Vector3(0, sign(delta.y), 0);
            else
                return Vector3(0, 0, sign(delta.z));
        }
    }

    RayCastingResult intersectionsWithRay(const Ray3D &ray) const
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
