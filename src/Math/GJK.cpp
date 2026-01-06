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
    // Single point or nothing. There is nothing to reduce
    if(size <= 1)
        return;
    // TODO: Implement this
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

    // Segment
    if(size == 2)
    {
        abort();
    }

    // Triangle
    if(size == 3)
    {
        abort();
    }

    assert(size == 4);
    abort();
}
void GJKVoronoiSimplexSolver::invalidateCache()
{
    hasComputedClosest = false;
}

} // End of namespace Math
} // End of namespace Woden
