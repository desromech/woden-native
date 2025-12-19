#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/AABox.hpp"

SUITE(AABox)
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

    TEST(InsertTwoPoints)
    {
        auto box = Woden::Math::AABox();
        box.insertPoint(Woden::Math::Vector3(1, 2, 3));
        box.insertPoint(Woden::Math::Vector3(1, -4, 1));
        CHECK_EQUAL(Woden::Math::Vector3(1, -4, 1), box.minCorner);
        CHECK_EQUAL(Woden::Math::Vector3(1, 2, 3), box.maxCorner);
    }

    TEST(RayCasting)
    {
        auto box = Woden::Math::AABox::WithHalfExtent(Woden::Math::Vector3(1, 2, 3));
        auto ray = Woden::Math::Ray3D::FromTo(Woden::Math::Vector3(0, 0, 10), Woden::Math::Vector3(0, 0, -10));
        auto result = box.intersectionsWithRay(ray);
        CHECK(!result.isEmpty());
        CHECK_EQUAL(size_t(2), result.intersectionPoints.size());
        CHECK_EQUAL(7, result.intersectionPoints[0]);
        CHECK_EQUAL(13, result.intersectionPoints[1]);
    }
}