#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Vector2.hpp"

SUITE(Vector2)
{
    TEST(Constructor)
    {
        CHECK_EQUAL(0, Woden::Math::Vector2().x);
        CHECK_EQUAL(0, Woden::Math::Vector2().y);

        CHECK_EQUAL(5, Woden::Math::Vector2(5).x);
        CHECK_EQUAL(5, Woden::Math::Vector2(5).y);

        CHECK_EQUAL(1, Woden::Math::Vector2(1, 2).x);
        CHECK_EQUAL(2, Woden::Math::Vector2(1, 2).y);
    }

    TEST(Cross)
    {
        auto v1 = Woden::Math::Vector2(1, 2);
        auto v2 = Woden::Math::Vector2(3, 7);
        CHECK_EQUAL(1, v1.cross(v2));
    }

    TEST(Dot)
    {
        auto v1 = Woden::Math::Vector2(1, 2);
        auto v2 = Woden::Math::Vector2(3, 7);
        CHECK_EQUAL(17, v1.dot(v2));
    }

    TEST(Addition)
    {
        auto v1 = Woden::Math::Vector2(1, 2);
        auto v2 = Woden::Math::Vector2(3, 4);
        auto v = v1 + v2;

        CHECK_EQUAL(4, v.x);
        CHECK_EQUAL(6, v.y);
    }

    TEST(Subtraction)
    {
        auto v1 = Woden::Math::Vector2(1, 2);
        auto v2 = Woden::Math::Vector2(3, 4);
        auto v = v1 - v2;

        CHECK_EQUAL(-2, v.x);
        CHECK_EQUAL(-2, v.y);
    }

    TEST(Mul)
    {
        auto v1 = Woden::Math::Vector2(1, 2);
        auto v2 = Woden::Math::Vector2(3, 4);
        auto v = v1 * v2;

        CHECK_EQUAL(3, v.x);
        CHECK_EQUAL(8, v.y);
    }

    TEST(Div)
    {
        auto v1 = Woden::Math::Vector2(1, 2);
        auto v2 = Woden::Math::Vector2(4, 8);
        auto v = v1 / v2;

        CHECK_EQUAL(0.25, v.x);
        CHECK_EQUAL(0.25, v.y);
    }
}