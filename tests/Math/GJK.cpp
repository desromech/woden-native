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

        CHECK_EQUAL(size_t(1), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
        CHECK(!simplex.containsOrigin());
    }

    TEST(SinglePoint2)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(0, 0, 0));

        CHECK_EQUAL(size_t(1), simplex.size);
        CHECK_EQUAL(Vector3(0, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
        CHECK(simplex.containsOrigin());
    }

    TEST(LineFirst)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(1, 0, 0));
        simplex.insertPoint(Vector3(2, 0, 0));

        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0, simplex.barycentricCoordinates[1]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(1), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
    }

    TEST(LineInside)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-1, 0, 0));
        simplex.insertPoint(Vector3(1, 0, 0));

        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(0, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
        CHECK(simplex.containsOrigin());
    }

    TEST(LineInside2)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(1, 0, 0));
        simplex.insertPoint(Vector3(-1, 0, 0));

        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(0, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
        CHECK(simplex.containsOrigin());
    }

    TEST(LineMiddle)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-1, 1, 0));
        simplex.insertPoint(Vector3(1, 1, 0));

        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(0, 1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();

        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
    }

    TEST(LineMiddle2)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(1, 1, 0));
        simplex.insertPoint(Vector3(-1, 1, 0));

        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(0, 1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();

        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
    }

    TEST(LineSecond)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-2, 0, 0));
        simplex.insertPoint(Vector3(-1, 0, 0));

        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(-1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(1, simplex.barycentricCoordinates[1]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(1), simplex.size);
        CHECK_EQUAL(Vector3(-1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
    }

    TEST(TriangleA)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(1, 0, 0));
        simplex.insertPoint(Vector3(3, 0, 0));
        simplex.insertPoint(Vector3(2, 1, 0));


        CHECK_EQUAL(size_t(3), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0, simplex.barycentricCoordinates[2]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(1), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
    }

    TEST(TriangleB)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(3, 0, 0));
        simplex.insertPoint(Vector3(1, 0, 0));
        simplex.insertPoint(Vector3(2, 1, 0));


        CHECK_EQUAL(size_t(3), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(1, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0, simplex.barycentricCoordinates[2]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(1), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
    }

    TEST(TriangleC)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(3, 0, 0));
        simplex.insertPoint(Vector3(2, 1, 0));
        simplex.insertPoint(Vector3(1, 0, 0));


        CHECK_EQUAL(size_t(3), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(1, simplex.barycentricCoordinates[2]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(1), simplex.size);
        CHECK_EQUAL(Vector3(1, 0, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(1, simplex.barycentricCoordinates[0]);
    }

    TEST(TriangleAB)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-1, 1, 0));
        simplex.insertPoint(Vector3(1,  1, 0));
        simplex.insertPoint(Vector3(0,  2, 0));

        CHECK_EQUAL(size_t(3), simplex.size);
        CHECK_EQUAL(Vector3(0, 1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0, simplex.barycentricCoordinates[2]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(-1, 1, 0), simplex.points[0]);
        CHECK_EQUAL(Vector3(1, 1, 0), simplex.points[1]);
        CHECK_EQUAL(Vector3(0, 1, 0), simplex.getClosestPointToOrigin());

        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
    }

    TEST(TriangleBC)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(0, 2, 0));
        simplex.insertPoint(Vector3(-1,  1, 0));
        simplex.insertPoint(Vector3(1, 1, 0));


        CHECK_EQUAL(size_t(3), simplex.size);
        CHECK_EQUAL(Vector3(0, 1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.0, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[2]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(-1, 1, 0), simplex.points[0]);
        CHECK_EQUAL(Vector3(1, 1, 0), simplex.points[1]);
        CHECK_EQUAL(Vector3(0, 1, 0), simplex.getClosestPointToOrigin());

        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
    }
    
    TEST(TriangleCA)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(1, 1, 0));
        simplex.insertPoint(Vector3(0, 2, 0));
        simplex.insertPoint(Vector3(-1, 1, 0));

        CHECK_EQUAL(size_t(3), simplex.size);
        CHECK_EQUAL(Vector3(0, 1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[2]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(2), simplex.size);
        CHECK_EQUAL(Vector3(1, 1, 0), simplex.points[0]);
        CHECK_EQUAL(Vector3(-1, 1, 0), simplex.points[1]);
        CHECK_EQUAL(Vector3(0, 1, 0), simplex.getClosestPointToOrigin());

        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[1]);
    }
    
    TEST(TriangleInside)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-1, -1, 0));
        simplex.insertPoint(Vector3(1, -1, 0));
        simplex.insertPoint(Vector3(0, 1, 0));

        CHECK_EQUAL(size_t(3), simplex.size);
        CHECK_EQUAL(Vector3::Zeros(), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[2]);
        CHECK(simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(3), simplex.size);
    }
    
    TEST(TetrahedronABC)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-1, -1, 1));
        simplex.insertPoint(Vector3(1, -1,  1));
        simplex.insertPoint(Vector3(0, -1, -1));
        simplex.insertPoint(Vector3(0, -2,  0));

        CHECK_EQUAL(size_t(4), simplex.size);
        CHECK_EQUAL(Vector3(0,-1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[2]);
        CHECK_EQUAL(0.0, simplex.barycentricCoordinates[3]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(3), simplex.size);
    }
    
    TEST(TetrahedronABD)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-1, -1, 1));
        simplex.insertPoint(Vector3(1, -1,  1));
        simplex.insertPoint(Vector3(0, -2, 0));
        simplex.insertPoint(Vector3(0, -1,  -1));

        CHECK_EQUAL(size_t(4), simplex.size);
        CHECK_EQUAL(Vector3(0,-1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0.0, simplex.barycentricCoordinates[2]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[3]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(3), simplex.size);
    }
    
    TEST(TetrahedronACD)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-1, -1, 1));
        simplex.insertPoint(Vector3(1, -2, 0));
        simplex.insertPoint(Vector3(1, -1, 1));
        simplex.insertPoint(Vector3(0, -1, -1));

        CHECK_EQUAL(size_t(4), simplex.size);
        CHECK_EQUAL(Vector3(0,-1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.0, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[2]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[3]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(3), simplex.size);
    }
    
    TEST(TetrahedronBCD)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(0, -2, 0));
        simplex.insertPoint(Vector3(-1, -1, 1));
        simplex.insertPoint(Vector3(1, -1, 1));
        simplex.insertPoint(Vector3(0, -1, -1));

        CHECK_EQUAL(size_t(4), simplex.size);
        CHECK_EQUAL(Vector3(0,-1, 0), simplex.getClosestPointToOrigin());
        CHECK_EQUAL(0.0, simplex.barycentricCoordinates[0]);
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[1]);
        CHECK_EQUAL(0.25, simplex.barycentricCoordinates[2]);
        CHECK_EQUAL(0.5, simplex.barycentricCoordinates[3]);
        CHECK(!simplex.containsOrigin());

        simplex.reduce();
        CHECK_EQUAL(size_t(3), simplex.size);
    }
    
    TEST(TetrahedronInside)
    {
        GJKVoronoiSimplexSolver simplex;
        simplex.insertPoint(Vector3(-1, -1, 1));
        simplex.insertPoint(Vector3( 1, -1, 1));
        simplex.insertPoint(Vector3( 0, -1, -1));
        simplex.insertPoint(Vector3( 0,  1, 0));

        CHECK_EQUAL(size_t(4), simplex.size);
        CHECK_EQUAL(Vector3(0,0, 0), simplex.getClosestPointToOrigin());
        CHECK(simplex.containsOrigin());
    }
}
