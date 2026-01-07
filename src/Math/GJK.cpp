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
            barycentricCoordinates[usedPointCoint] = barycentricCoordinates[i];
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
    {
        computeClosestToLine();
        hasComputedClosest = true;
        return;
    }

    // Triangle
    if(size == 3)
    {
        computeClosestToTriangle();
        hasComputedClosest = true;
        return;
    }

    // Tetrahedron
    if(size == 4)
    {
        computeClosestToTetrahedron();
        hasComputedClosest = true;
        return;
    }
    
    // Unsupported case.
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

void GJKVoronoiSimplexSolver::computeClosestToTriangle()
{
	// Algorithm from 'Real Time Collision detection' by Ericson. ClosestPtPointTriangle.
	auto a = points[0];
	auto b = points[1];
	auto c = points[2];
	auto p = Vector3::Zeros();

    // P on AB.
	auto snom = (p - a).dot(b - a);
	auto sdenom = (p - b).dot(a - b);
	
	// P on AC.
	auto tnom = (p - a).dot(c - a);
	auto tdenom = (p - c).dot(a - c);
	
	//Check the vertex region.
	if (snom <= 0.0 && tnom <= 0.0)
    {
        barycentricCoordinates[0] = 1;
        barycentricCoordinates[1] = 0;
        barycentricCoordinates[2] = 0;

        usedPoints[0] = true;
        usedPoints[1] = false;
        usedPoints[2] = false;

		closestPointToOrigin = a;
		return;
    }
	
    // P on BC
	auto unom = (p - b).dot(c - b);
	auto udenom = (p - c).dot(b - c);
	if (sdenom <= 0.0 && unom <= 0.0)
    {
        barycentricCoordinates[0] = 0;
        barycentricCoordinates[1] = 1;
        barycentricCoordinates[2] = 0;

        usedPoints[0] = false;
        usedPoints[1] = true;
        usedPoints[2] = false;

		closestPointToOrigin = b;
		return;
    }

	if(tdenom <= 0.0 && udenom <= 0.0)
    {
        barycentricCoordinates[0] = 0;
        barycentricCoordinates[1] = 0;
        barycentricCoordinates[2] = 1;

        usedPoints[0] = false;
        usedPoints[1] = false;
        usedPoints[2] = true;

		closestPointToOrigin = c;
        return;
    }
	
	auto n = (b - a).cross(c - a);
	
	auto vc = n.dot((a - p).cross(b - p));
	if(vc <= 0.0 && snom >= 0.0 && sdenom >= 0.0)
    {
		auto lambda = snom / (snom + sdenom);
		closestPointToOrigin = a + (Vector3(lambda) * (b - a));
        
        barycentricCoordinates[0] = 1 - lambda;
        barycentricCoordinates[1] = lambda;
        barycentricCoordinates[2] = 0;

        usedPoints[0] = true;
        usedPoints[1] = true;
        usedPoints[2] = false;
        return;
    }

	auto va = n.dot((b - p).cross(c - p));
	if (va <= 0.0 && unom >= 0.0 && udenom >= 0.0){
		auto lambda = unom / (unom + udenom);
        closestPointToOrigin = b + (Vector3(lambda) * (c - b));
        
        barycentricCoordinates[0] = 0;
        barycentricCoordinates[1] = 1 - lambda;
        barycentricCoordinates[2] = lambda;

        usedPoints[0] = false;
        usedPoints[1] = true;
        usedPoints[2] = true;
		return;
	}

	auto vb = n.dot((c - p).cross(a - p));
	if (vb <= 0.0 && tnom >= 0.0 && tdenom >= 0.0)
    {
		auto lambda = tnom / (tnom + tdenom);
		closestPointToOrigin = a + (Vector3(lambda) * (c - a));
        
        barycentricCoordinates[0] = 1 - lambda;
        barycentricCoordinates[1] = 0;
        barycentricCoordinates[2] = lambda;

        usedPoints[0] = true;
        usedPoints[1] = false;
        usedPoints[2] = true;
        return;
    }
	
	auto u = va / (va + vb + vc);
	auto v = vb / (va + vb + vc);
	auto w = 1 - u - v;

    barycentricCoordinates[0] = u;
    barycentricCoordinates[1] = v;
    barycentricCoordinates[2] = w;

    usedPoints[0] = true;
    usedPoints[1] = true;
    usedPoints[2] = true;

    closestPointToOrigin = (a*u) + (b*v) + (c*w);
}

Scalar pointSideOfPlane(Vector3 p, Vector3 p4, /*sideOfPlane*/ Vector3 p1, Vector3 p2, Vector3 p3)
{
	auto u = p2 - p1;
	auto v = p3 - p1;
	auto n = u.cross(v);
	auto d = p1.dot(n);
	auto signP = Math::sign(((p.dot(n)) - d));
	auto signP4 = Math::sign(((p4.dot(n)) - d));
	return signP*signP4;
}

void GJKVoronoiSimplexSolver::computeClosestToTetrahedron()
{
	auto a = points[0];
	auto b = points[1];
	auto c = points[2];
	auto d = points[3];
	auto p = Vector3::Zeros();
	
	auto abcSide = pointSideOfPlane(p, d, /*sideOfPlane*/ a, b, c);
	auto abdSide = pointSideOfPlane(p, c, /*sideOfPlane*/ a, b, d);
	auto acdSide = pointSideOfPlane(p, b, /*sideOfPlane*/ a, c, d);
	auto bcdSide = pointSideOfPlane(p, a, /*sideOfPlane*/ b, c, d);

	// Is inside?
	if(abcSide >= 0 && abdSide >= 0 && acdSide >= 0 && bcdSide >= 0)
    {
        closestPointToOrigin = Vector3::Zeros();

        barycentricCoordinates[0] = 1;
        barycentricCoordinates[1] = 1;
        barycentricCoordinates[2] = 1;
        barycentricCoordinates[3] = 1;

        usedPoints[0] = true;
        usedPoints[1] = true;
        usedPoints[2] = true;
        usedPoints[3] = true;
		return;
    }

    Scalar bestResult = INFINITY;

	if(abcSide <= 0)
    {
        GJKVoronoiSimplexSolver subSimplex;
        subSimplex.insertPoint(a);
        subSimplex.insertPoint(b);
        subSimplex.insertPoint(c);
        subSimplex.computeClosestToTriangle();

		auto newResult = subSimplex.closestPointToOrigin.length2();
		if(newResult < bestResult)
        {
			bestResult = newResult;
			auto &subBarycentric = subSimplex.barycentricCoordinates;
			closestPointToOrigin = subSimplex.closestPointToOrigin;
			
            barycentricCoordinates[0] = subBarycentric[0];
            barycentricCoordinates[1] = subBarycentric[1];
            barycentricCoordinates[2] = subBarycentric[2];
            barycentricCoordinates[3] = 0;

            usedPoints[0] = true;
            usedPoints[1] = true;
            usedPoints[2] = true;
            usedPoints[3] = false;
        }
    }

	if(abdSide <= 0)
    {
        GJKVoronoiSimplexSolver subSimplex;
        subSimplex.insertPoint(a);
        subSimplex.insertPoint(b);
        subSimplex.insertPoint(d);
        subSimplex.computeClosestToTriangle();

		auto newResult = subSimplex.closestPointToOrigin.length2();
		if(newResult < bestResult)
        {
			bestResult = newResult;
			auto &subBarycentric = subSimplex.barycentricCoordinates;
			closestPointToOrigin = subSimplex.closestPointToOrigin;
			
            barycentricCoordinates[0] = subBarycentric[0];
            barycentricCoordinates[1] = subBarycentric[1];
            barycentricCoordinates[2] = 0;
            barycentricCoordinates[3] = subBarycentric[2];

            usedPoints[0] = true;
            usedPoints[1] = true;
            usedPoints[2] = false;
            usedPoints[3] = true;
        }
    }
    
	if(acdSide <= 0)
    {
        GJKVoronoiSimplexSolver subSimplex;
        subSimplex.insertPoint(a);
        subSimplex.insertPoint(c);
        subSimplex.insertPoint(d);
        subSimplex.computeClosestToTriangle();

		auto newResult = subSimplex.closestPointToOrigin.length2();
		if(newResult < bestResult)
        {
			bestResult = newResult;
			auto &subBarycentric = subSimplex.barycentricCoordinates;
			closestPointToOrigin = subSimplex.closestPointToOrigin;
			
            barycentricCoordinates[0] = subBarycentric[0];
            barycentricCoordinates[1] = 0;
            barycentricCoordinates[2] = subBarycentric[1];
            barycentricCoordinates[3] = subBarycentric[2];

            usedPoints[0] = true;
            usedPoints[1] = false;
            usedPoints[2] = true;
            usedPoints[3] = true;
        }
    }
    
    /*

	bcdSide <= 0 ifTrue: [ 
		subSimplex := self class new
			points: { b . c . d };
			computeClosestToTriangle.
		newResult := subSimplex closestPointToOrigin length2.
		newResult < bestResult ifTrue: [
			bestResult := newResult.
			subBarycentric := subSimplex barycentricCoordinates.
			closestPointToOrigin := subSimplex closestPointToOrigin.
			barycentricCoordinates := {0 . subBarycentric first . subBarycentric second . subBarycentric third}.
			usedPoints := #(false true true true).
		]
	].	
*/
	if(bcdSide <= 0)
    {
        GJKVoronoiSimplexSolver subSimplex;
        subSimplex.insertPoint(b);
        subSimplex.insertPoint(c);
        subSimplex.insertPoint(d);
        subSimplex.computeClosestToTriangle();

		auto newResult = subSimplex.closestPointToOrigin.length2();
		if(newResult < bestResult)
        {
			bestResult = newResult;
			auto &subBarycentric = subSimplex.barycentricCoordinates;
			closestPointToOrigin = subSimplex.closestPointToOrigin;
			
            barycentricCoordinates[0] = 0;
            barycentricCoordinates[1] = subBarycentric[0];
            barycentricCoordinates[2] = subBarycentric[1];
            barycentricCoordinates[3] = subBarycentric[2];

            usedPoints[0] = false;
            usedPoints[1] = true;
            usedPoints[2] = true;
            usedPoints[3] = true;
        }
    }

    if(bestResult == INFINITY)
    {
        fprintf(stderr, "Unsupported case.\n");
        abort();
    }
}

void GJKVoronoiSimplexSolver::invalidateCache()
{
    hasComputedClosest = false;
}

} // End of namespace Math
} // End of namespace Woden
