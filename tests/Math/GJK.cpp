#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/GJK.hpp"

using namespace Woden::Math;

SUITE(GJKVoronoiSimplexSolver)
{
    TEST(Empty)
    {
        GJKVoronoiSimplexSolver simplex;
        CHECK(simplex.empty());
        simplex.reduce();
        CHECK(simplex.empty());
    }

    TEST(SinglePoint)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(1, 0, 0));

        CHECK_EQUAL(1, simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
        CHECK(!simplex.containsOrigin());
    }

    TEST(SinglePoint2)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(0, 0, 0));

        CHECK_EQUAL(1, simplex.size);
        CHECK_EQUAL(Vector3(0, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
        CHECK(simplex.containsOrigin());
    }

    TEST(LineFirst)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(1, 0, 0));
        simplex.insertPoint(Vector3(2, 0, 0));

        CHECK_EQUAL(2, simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0, simplex.barycentricCoordinates[1]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(1, simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
    }
}
