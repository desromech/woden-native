#ifndef WODEN_MATH_GJK_HPP
#define WODEN_MATH_GJK_HPP

#include "Vector3.hpp"
#include "Ray3D.hpp"
#include <stddef.h>
#include <array>
#include <optional>

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
        if(empty())
            return false;

        if(!hasComputedClosest)
            computeClosestToOrigin();
        return closeTo(closestPointToOrigin, Vector3::Zeros());
    }

    bool containsPoint(const Math::Vector3 &point)
    {
        if (empty())
            return false;
        for (size_t i = 0 ; i < size; ++i)
        {
            if(closeTo(point, points[i]))
                return true;
        }

        return false;
    }

    Vector3 getClosestPointToOrigin()
    {
        if(!hasComputedClosest)
            computeClosestToOrigin();
        return closestPointToOrigin;
    }

    Vector3 getClosestPointToOriginInFirst()
    {
        if(!hasComputedClosest)
            computeClosestToOrigin();

        Vector3 result = Vector3::Zeros();
        Scalar barycentricSum = 0;
        for(size_t i = 0; i < size; ++i)
        {
            result += firstPoints[i]*barycentricCoordinates[i];
            barycentricSum += barycentricCoordinates[i];
        }
        return result / barycentricSum;
    }


    Vector3 getClosestPointToOriginInSecond()
    {
        if(!hasComputedClosest)
            computeClosestToOrigin();

        Vector3 result = Vector3::Zeros();
        Scalar barycentricSum = 0;
        for(size_t i = 0; i < size; ++i)
        {
            result += secondPoints[i]*barycentricCoordinates[i];
            barycentricSum += barycentricCoordinates[i];
        }
        return result / barycentricSum;
    }

    void insertPoint(const Vector3 &point);
    void insertPointWithFirstAndSecond(const Vector3 &point, const Vector3 &firstPoint, const Vector3 &secondPoint);
    void reduce();

    template<typename FT>
    void transformPointsWith(FT &&transform)
    {
        for(size_t i = 0; i < size; ++i)
        {
            points[i] = transform(points[i]);
            firstPoints[i] = transform(firstPoints[i]);
            secondPoints[i] = transform(secondPoints[i]);
        }
        
        invalidateCache();
    }

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
    const Scalar Epsilon = 0.0001f;
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

template<typename SF>
std::optional<std::pair<Scalar, Vector3>> computeGJKRayCasting(const Ray3D &ray, SF&& supportFunction)
{
    // Algorithm from 'Ray Casting against General Convex Objectswith Application to Continuous CollisionDetection' by G. Van Den Bergen."

    const auto MaxNumberOfIterations = 32;
	const float Epsilon = 0.00001;
	const float Epsilon2 = Epsilon*Epsilon;

    auto lambda = ray.tmin;
    auto lambdaMax = ray.tmax;

	auto s = ray.origin;
	auto r = ray.direction;
	auto x = s + (r * lambda);
	auto n = Vector3::Zeros();

    Math::Vector3 v = x - supportFunction(-r);
	GJKVoronoiSimplexSolver simplex;
	auto remainingIterations = MaxNumberOfIterations;

    while (remainingIterations > 0 && v.length2() > Epsilon2)
    {
		if(lambda > lambdaMax)
            return std::nullopt;

        auto p = supportFunction(v);
		Math::Vector3 w = x - p;
		auto VdotW = v.dot(w);
        if(VdotW > 0)
        {
			auto VdotR = v.dot(r);
			if(VdotR >= -Epsilon2)
                return std::nullopt;
                
            lambda = lambda - (VdotW / VdotR);
			if(lambda > lambdaMax)
                return std::nullopt;

            auto oldX = x;
			x = s + (r * lambda);
			n = v;
			w = x - p;
			auto deltaX = x - oldX;

            simplex.transformPointsWith([&](const Math::Vector3 &sp){
                return sp + deltaX;
            });

            if (simplex.containsOrigin())
                return std::make_pair(lambda, n);
            simplex.reduce();
        }


        if (!simplex.containsPoint(w))
            simplex.insertPoint(w);

        if (simplex.containsOrigin())
        {
            remainingIterations = 0;
        }
        else
        {
            v = simplex.getClosestPointToOrigin();
            simplex.reduce();
            --remainingIterations;
        }
    }

    return std::make_pair(lambda, n);
}


extern Vector3 PenetrationDistanceSampleVector[26];

struct PenetrationDistanceSamplingResult
{
    Scalar distance = INFINITY;
    Vector3 normal = Vector3::Zeros();
    Vector3 firstPoint = Vector3::Zeros();
    Vector3 secondPoint = Vector3::Zeros();
};

template<typename FS, typename SS>
std::optional<PenetrationDistanceSamplingResult> samplePenetrationDistanceAndNormal(FS&& firstSupport, SS&& secondSupport, const Vector3 &separatingAxisHint)
{
    PenetrationDistanceSamplingResult bestResult;

    auto&& sampleBlock = [&](const Vector3 &sampleVector){
        Vector3 firstPoint = firstSupport(-sampleVector);
        Vector3 secondPoint = secondSupport(sampleVector);
        
        auto supportVector = secondPoint - firstPoint;
        auto delta = supportVector.dot(sampleVector);
        if(delta < bestResult.distance)
        {
            bestResult.distance = delta;
			bestResult.normal = sampleVector;
			bestResult.firstPoint = firstPoint;
			bestResult.secondPoint = secondPoint;
        }
    };

    sampleBlock(separatingAxisHint);
    sampleBlock(-separatingAxisHint);
    for(const auto &sampleVector : PenetrationDistanceSampleVector)
        sampleBlock(sampleVector);

    if(bestResult.distance < 0)
        return std::nullopt;
    return bestResult;
}

template<typename FS, typename SS>
std::optional<PenetrationDistanceSamplingResult> samplePenetrationDistanceAndNormalWithMargin(FS&& firstSupport, SS&& secondSupport, Scalar margin, const Vector3 &separatingAxisHint)
{
    auto optSamplingResult = samplePenetrationDistanceAndNormal(firstSupport, secondSupport, separatingAxisHint);
    if(!optSamplingResult.has_value())
        return std::nullopt;

    auto samplingResult = optSamplingResult.value();

    auto distance = samplingResult.distance;
    auto normal = samplingResult.normal;

    auto extraSeparation = 0.5f;
    auto distanceWithMargin = extraSeparation + distance + margin;
    auto displacement = normal * distanceWithMargin;

    auto&& displacedFirstSupportFunction = [&](const Vector3 &D) {
        return firstSupport(D) + displacement;
    };

    auto gjkSimplex = computeGJKSimplex(displacedFirstSupportFunction, secondSupport, separatingAxisHint);
    auto separationVector = gjkSimplex.getClosestPointToOrigin();
    auto separationVectorLength = separationVector.length();

    auto correctedPenetrationDistance = distanceWithMargin - separationVectorLength + margin;

    auto firstClosestPoint = gjkSimplex.getClosestPointToOriginInFirst();
    auto secondClosestPoint = gjkSimplex.getClosestPointToOriginInSecond();

    firstClosestPoint = firstClosestPoint - normal*Vector3(distanceWithMargin);

    PenetrationDistanceSamplingResult correctedResult;
    correctedResult.distance = correctedPenetrationDistance;
    correctedResult.normal = normal;
    correctedResult.firstPoint = firstClosestPoint;
    correctedResult.secondPoint = secondClosestPoint;
    return correctedResult;
}

} // End of namespace Math
} // End of namespace Woden

#endif //WODEN_MATH_GJK_HPP