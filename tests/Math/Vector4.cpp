#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Vector4.hpp"

SUITE(Vector4)
{
    TEST(Constructor)
    {
        CHECK_EQUAL(0, Woden::Math::Vector4().x);
        CHECK_EQUAL(0, Woden::Math::Vector4().y);
        CHECK_EQUAL(0, Woden::Math::Vector4().z);

        CHECK_EQUAL(5, Woden::Math::Vector4(5).x);
        CHECK_EQUAL(5, Woden::Math::Vector4(5).y);
        CHECK_EQUAL(5, Woden::Math::Vector4(5).z);
        CHECK_EQUAL(5, Woden::Math::Vector4(5).w);

        CHECK_EQUAL(1, Woden::Math::Vector4(1, 2, 3, 4).x);
        CHECK_EQUAL(2, Woden::Math::Vector4(1, 2, 3, 4).y);
        CHECK_EQUAL(3, Woden::Math::Vector4(1, 2, 3, 4).z);
        CHECK_EQUAL(4, Woden::Math::Vector4(1, 2, 3, 4).w);
    }

    TEST(Addition)
    {
        auto v1 = Woden::Math::Vector4(1, 2, 3, 5);
        auto v2 = Woden::Math::Vector4(6, 7, 8, 9);
        auto v = v1 + v2;

        CHECK_EQUAL(7, v.x);
        CHECK_EQUAL(9, v.y);
        CHECK_EQUAL(11, v.z);
        CHECK_EQUAL(14, v.w);
    }

    TEST(Subtraction)
    {
        auto v1 = Woden::Math::Vector4(1, 2, 3, 4);
        auto v2 = Woden::Math::Vector4(5, 6, 7, 8);
        auto v = v1 - v2;

        CHECK_EQUAL(-4, v.x);
        CHECK_EQUAL(-4, v.y);
        CHECK_EQUAL(-4, v.z);
        CHECK_EQUAL(-4, v.w);
    }

    TEST(Mul)
    {
        auto v1 = Woden::Math::Vector4(1, 2, 3, 5);
        auto v2 = Woden::Math::Vector4(6, 7, 8, 9);
        auto v = v1 * v2;

        CHECK_EQUAL(6, v.x);
        CHECK_EQUAL(14, v.y);
        CHECK_EQUAL(24, v.z);
        CHECK_EQUAL(45, v.w);
    }

    TEST(Div)
    {
        auto v1 = Woden::Math::Vector4(1, 2, 3, 5);
        auto v2 = Woden::Math::Vector4(2, 4, 6, 10);
        auto v = v1 / v2;

        CHECK_EQUAL(0.5, v.x);
        CHECK_EQUAL(0.5, v.y);
        CHECK_EQUAL(0.5, v.z);
        CHECK_EQUAL(0.5, v.w);
    }
}