#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Scalar.hpp"

SUITE(Scalar)
{
    TEST(Abs)
    {
        CHECK_EQUAL(1.0, abs(-1));
    }
}
