#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Ray3D.hpp"

SUITE(Ray3D)
{
    TEST(Constructor)
    {
        auto startPoint = Woden::Math::Vector3(0, 0, 0);
        auto endPoint = Woden::Math::Vector3(0, 0, -10);
        auto ray = Woden::Math::Ray3D::FromTo(startPoint, endPoint);

        CHECK_EQUAL(Woden::Math::Vector3(0), ray.origin);
        CHECK_EQUAL(Woden::Math::Vector3(0, 0, -1), ray.direction);
        CHECK_EQUAL(0, ray.tmin);
        CHECK_EQUAL(10, ray.tmax);
    }
}