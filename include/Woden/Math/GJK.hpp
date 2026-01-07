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
    void computeClosestToTetrahedron();
    void invalidateCache();

    bool hasComputedClosest = false;
    Vector3 closestPointToOrigin;
};

template<typename FS, typename SS>
GJKVoronoiSimplexSolver computeGJKSimplex(FS&& firstSupportFunction, SS&& secondSupportFunction, const Vector3 &startingDirectionHint)
{
    const int MaxNumberOfIterations = 32;
    const Scalar Epsilon = 0.00001;
    GJKVoronoiSimplexSolver simplex;

    auto nextDirection = startingDirectionHint;
	auto firstSupportPoint = firstSupportFunction(nextDirection);
	auto secondSupportPoint = secondSupportFunction(-nextDirection);
    auto lastPoint = firstSupportPoint - secondSupportPoint;
	simplex.insertPointWithFirstAndSecond(lastPoint, firstSupportPoint, secondSupportPoint);
	nextDirection = -lastPoint;

    for(int remainingIterations = MaxNumberOfIterations; remainingIterations > 0; --remainingIterations)
    {
		firstSupportPoint = firstSupportFunction(nextDirection);
		secondSupportPoint = secondSupportFunction(-nextDirection);

        auto nextPoint = firstSupportPoint - secondSupportPoint;

        // Are we getting closer to the origin?
		auto delta = nextPoint - lastPoint;
		if (delta.dot(nextDirection) <= Epsilon)
            return simplex;

		simplex.insertPointWithFirstAndSecond(nextPoint, firstSupportPoint, secondSupportPoint);
		lastPoint = nextPoint;

		// Do we contain the origin?
		if(simplex.containsOrigin())
            return simplex;

		// Advance in direction to the origin.
		nextDirection = -simplex.getClosestPointToOrigin();
		
        // Reduce the simplex
		simplex.reduce();
    }

    return simplex;
}

template<typename FS, typename SS>
GJKVoronoiSimplexSolver computeGJKSimplex(FS&& firstSupportFunction, SS&& secondSupportFunction)
{
    return computeGJKSimplex(firstSupportFunction, secondSupportFunction, Vector3(1, 0, 0));
}

template<typename FS, typename SS>
Scalar computeGJKDistance(FS&& firstSupportFunction, SS&& secondSupportFunction)
{
    return computeGJKSimplex(firstSupportFunction, secondSupportFunction).getClosestPointToOrigin().length();
}


} // End of namespace Math
} // End of namespace Woden

#endif //WODEN_MATH_GJK_HPP