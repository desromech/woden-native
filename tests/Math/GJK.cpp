#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/GJK.hpp"

SUITE(GJKVoronoiSimplexSolver)
{
    TEST(Empty)
    {
        auto solver = Woden::Math::GJKVoronoiSimplexSolver();
        CHECK(solver.empty());
        solver.reduce();
        CHECK(solver.empty());
    }
}
