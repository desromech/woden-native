#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Quaternion.hpp"

SUITE(Quaternion)
{
    TEST(Constructor)
    {
        CHECK_EQUAL(0, Woden::Math::Quaternion().x);
        CHECK_EQUAL(0, Woden::Math::Quaternion().y);
        CHECK_EQUAL(0, Woden::Math::Quaternion().z);
        CHECK_EQUAL(0, Woden::Math::Quaternion().w);

        CHECK_EQUAL(0, Woden::Math::Quaternion::Identity().x);
        CHECK_EQUAL(0, Woden::Math::Quaternion::Identity().y);
        CHECK_EQUAL(0, Woden::Math::Quaternion::Identity().z);
        CHECK_EQUAL(1, Woden::Math::Quaternion::Identity().w);

        CHECK_EQUAL(1, Woden::Math::Quaternion(1, 2, 3, 4).x);
        CHECK_EQUAL(2, Woden::Math::Quaternion(1, 2, 3, 4).y);
        CHECK_EQUAL(3, Woden::Math::Quaternion(1, 2, 3, 4).z);
        CHECK_EQUAL(4, Woden::Math::Quaternion(1, 2, 3, 4).w);
    }

    TEST(Addition)
    {
        auto v1 = Woden::Math::Quaternion(1, 2, 3, 5);
        auto v2 = Woden::Math::Quaternion(6, 7, 8, 9);
        auto v = v1 + v2;

        CHECK_EQUAL(7, v.x);
        CHECK_EQUAL(9, v.y);
        CHECK_EQUAL(11, v.z);
        CHECK_EQUAL(14, v.w);
    }

    TEST(Subtraction)
    {
        auto v1 = Woden::Math::Quaternion(1, 2, 3, 4);
        auto v2 = Woden::Math::Quaternion(5, 6, 7, 8);
        auto v = v1 - v2;

        CHECK_EQUAL(-4, v.x);
        CHECK_EQUAL(-4, v.y);
        CHECK_EQUAL(-4, v.z);
        CHECK_EQUAL(-4, v.w);
    }

    TEST(RotationAsMatrix)
    {
        CHECK(Woden::Math::closeTo(
            Woden::Math::Matrix3x3::Identity(),
            Woden::Math::Quaternion::Identity().asMatrix())
        );

        CHECK(Woden::Math::closeTo(
            Woden::Math::Matrix3x3::XRotation(1),
            Woden::Math::Quaternion::XRotation(1).asMatrix())
        );

        CHECK(Woden::Math::closeTo(
            Woden::Math::Matrix3x3::YRotation(1),
            Woden::Math::Quaternion::YRotation(1).asMatrix())
        );

        CHECK(Woden::Math::closeTo(
            Woden::Math::Matrix3x3::ZRotation(1),
            Woden::Math::Quaternion::ZRotation(1).asMatrix())
        );
    }

}