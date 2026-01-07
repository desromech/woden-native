#include "Woden/Math/GJK.hpp"
#include <assert.h>

namespace Woden
{
namespace Math
{
void GJKVoronoiSimplexSolver::insertPoint(const Vector3 &point)
{
    insertPointWithFirstAndSecond(point, point, point);
}

void GJKVoronoiSimplexSolver::insertPointWithFirstAndSecond(const Vector3 &point, const Vector3 &firstPoint, const Vector3 &secondPoint)
{
    assert(size < Capacity);
    points[size] = point;
    firstPoints[size] = firstPoint;
    secondPoints[size] = secondPoint;
    ++size;
    invalidateCache();
}

void GJKVoronoiSimplexSolver::reduce()
{
    if(!hasComputedClosest)
        computeClosestToOrigin();

    // Single point or nothing. There is nothing to reduce
    if(size <= 1)
        return;

    size_t usedPointCoint = 0;
    for(size_t i = 0; i < size; ++i)
    {
        if(usedPoints[i])
        {
            points[usedPointCoint]       = points[i];
            firstPoints[usedPointCoint]  = firstPoints[i];
            secondPoints[usedPointCoint] = secondPoints[i];
            usedPoints[usedPointCoint]   = usedPoints[i];
            ++usedPointCoint;
        }
    }

    size = usedPointCoint;
}

void GJKVoronoiSimplexSolver::computeClosestToOrigin()
{
    // Nothing
    if(empty())
    {
        closestPointToOrigin = Vector3();
        hasComputedClosest = true;
        return;
    }

    // Single point
    if(size == 1)
    {
        closestPointToOrigin = points[size - 1];
        barycentricCoordinates[0] = 1;
        hasComputedClosest = true;
        return;
    }

    // Line
    if(size == 2)
        return computeClosestToLine();

    // Triangle
    if(size == 3)
    {
        abort();
    }

    assert(size == 4);
    abort();
}

void GJKVoronoiSimplexSolver::computeClosestToLine()
{
    auto from = points[0];
    auto to = points[1];

    auto delta = to - from;
    auto lambda = (Vector3::Zeros() - from).dot(delta);
    if (lambda <= 0)
    {
        barycentricCoordinates[0] = 1;
        barycentricCoordinates[1] = 0;
        usedPoints[0] = true;
        usedPoints[1] = false;
        closestPointToOrigin = from;
        return;
    }

    lambda = lambda / delta.length2();
    if(lambda >= 1)
    {
        barycentricCoordinates[0] = 0;
        barycentricCoordinates[1] = 1;
        usedPoints[0] = false;
        usedPoints[1] = true;
        closestPointToOrigin = to;
        return;
    }

    closestPointToOrigin = from + (delta*lambda);
    barycentricCoordinates[0] = 1 - lambda;
    barycentricCoordinates[1] = lambda;
    usedPoints[0] = true;
    usedPoints[1] = true;
}

void GJKVoronoiSimplexSolver::invalidateCache()
{
    hasComputedClosest = false;
}

} // End of namespace Math
} // End of namespace Woden
