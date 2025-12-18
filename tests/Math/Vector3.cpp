#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Vector3.hpp"

SUITE(Vector3)
{
    TEST(Constructor)
    {
        CHECK_EQUAL(0, Woden::Math::Vector3().x);
        CHECK_EQUAL(0, Woden::Math::Vector3().y);
        CHECK_EQUAL(0, Woden::Math::Vector3().z);

        CHECK_EQUAL(5, Woden::Math::Vector3(5).x);
        CHECK_EQUAL(5, Woden::Math::Vector3(5).y);
        CHECK_EQUAL(5, Woden::Math::Vector3(5).z);

        CHECK_EQUAL(1, Woden::Math::Vector3(1, 2, 3).x);
        CHECK_EQUAL(2, Woden::Math::Vector3(1, 2, 3).y);
        CHECK_EQUAL(3, Woden::Math::Vector3(1, 2, 3).z);
    }

    TEST(Dot)
    {
        auto v1 = Woden::Math::Vector3(1, 2, 3);
        auto v2 = Woden::Math::Vector3(4, 5, 6);
        CHECK_EQUAL(4+10+18, v1.dot(v2));
    }

    TEST(Addition)
    {
        auto v1 = Woden::Math::Vector3(1, 2, 3);
        auto v2 = Woden::Math::Vector3(4, 5, 6);
        auto v = v1 + v2;

        CHECK_EQUAL(5, v.x);
        CHECK_EQUAL(7, v.y);
        CHECK_EQUAL(9, v.z);
    }

    TEST(Subtraction)
    {
        auto v1 = Woden::Math::Vector3(1, 2, 3);
        auto v2 = Woden::Math::Vector3(4, 5, 6);
        auto v = v1 - v2;

        CHECK_EQUAL(-3, v.x);
        CHECK_EQUAL(-3, v.y);
        CHECK_EQUAL(-3, v.z);
    }

    TEST(Mul)
    {
        auto v1 = Woden::Math::Vector3(1, 2, 3);
        auto v2 = Woden::Math::Vector3(4, 5, 6);
        auto v = v1 * v2;

        CHECK_EQUAL(4, v.x);
        CHECK_EQUAL(10, v.y);
        CHECK_EQUAL(18, v.z);
    }

    TEST(Div)
    {
        auto v1 = Woden::Math::Vector3(1, 2, 3);
        auto v2 = Woden::Math::Vector3(4, 4, 6);
        auto v = v1 / v2;

        CHECK_EQUAL(0.25, v.x);
        CHECK_EQUAL(0.5, v.y);
        CHECK_EQUAL(0.5, v.z);
    }
}