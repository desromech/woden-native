#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/AABox.hpp"

SUITE(Ray3D)
{
    TEST(EmptyConstructor)
    {
        CHECK(Woden::Math::AABox().isEmpty());
    }

    TEST(WithHalfExtent)
    {
        auto box = Woden::Math::AABox::WithHalfExtent(Woden::Math::Vector3(1, 2, 3));
        CHECK_EQUAL(Woden::Math::Vector3(-1, -2, -3), box.minCorner);
        CHECK_EQUAL(Woden::Math::Vector3(1, 2, 3), box.maxCorner);

    }

    TEST(InsertSinglePoint)
    {
        auto box = Woden::Math::AABox();
        box.insertPoint(Woden::Math::Vector3(1, 2, 3));
        CHECK_EQUAL(Woden::Math::Vector3(1, 2, 3), box.minCorner);
        CHECK_EQUAL(Woden::Math::Vector3(1, 2, 3), box.maxCorner);
    }

}