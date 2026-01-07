#ifndef WODEN_MATH_GJK_HPP
#define WODEN_MATH_GJK_HPP

#include "Vector3.hpp"
#include <stddef.h>
#include <array>

namespace Woden
{
namespace Math
{

/**
 * A voronoi region simplex solver
 */
class GJKVoronoiSimplexSolver
{
public:
    static const size_t Capacity = 4;
    
    bool empty() const
    {
        return size == 0;
    }

    bool containsOrigin()
    {
        if(!hasComputedClosest)
            computeClosestToOrigin();
        return closestPointToOrigin == Vector3::Zeros();
    }

    Vector3 getClosestPointToOrigin()
    {
        if(!hasComputedClosest)
            computeClosestToOrigin();
        return closestPointToOrigin;
    }

    void insertPoint(const Vector3 &point);
    void insertPointWithFirstAndSecond(const Vector3 &point, const Vector3 &firstPoint, const Vector3 &secondPoint);
    void reduce();

    size_t size = 0;
    std::array<Vector3, Capacity> points;
    std::array<Vector3, Capacity> firstPoints;
    std::array<Vector3, Capacity> secondPoints;
    std::array<Scalar, Capacity> barycentricCoordinates;
    std::array<bool, Capacity> usedPoints;
    
private:
    void computeClosestToOrigin();
    void computeClosestToLine();
    void computeClosestToTriangle();
    void invalidateCache();

    bool hasComputedClosest = false;
    Vector3 closestPointToOrigin;
};

} // End of namespace Math
} // End of namespace Woden

#endif //WODEN_MATH_GJK_HPP