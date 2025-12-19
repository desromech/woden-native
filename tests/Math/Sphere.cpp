#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Sphere.hpp"

SUITE(Sphere)
{
    TEST(EmptyConstructor)
    {
        auto sphere = Woden::Math::Sphere();
        CHECK_EQUAL(Woden::Math::Vector3(0), sphere.center);
        CHECK_EQUAL(0, sphere.radius);
    }

    TEST(RayCasting)
    {
        auto sphere = Woden::Math::Sphere(Woden::Math::Vector3(0), 3);
        auto ray = Woden::Math::Ray3D::FromTo(Woden::Math::Vector3(0, 0, 10), Woden::Math::Vector3(0, 0, -10));
        auto result = sphere.intersectionsWithRay(ray);
        CHECK(!result.isEmpty());
        CHECK_EQUAL(size_t(2), result.intersectionPoints.size());
        CHECK_EQUAL(7, result.intersectionPoints[0]);
        CHECK_EQUAL(13, result.intersectionPoints[1]);
    }
}