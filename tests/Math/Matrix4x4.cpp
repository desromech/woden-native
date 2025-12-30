#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Matrix4x4.hpp"

SUITE(Matrix4x4)
{
    TEST(Constructor)
    {
        auto mat = Woden::Math::Matrix4x4(
            1,  2,  3,  4,
            5,  6,  7,  8,
            9,  10, 11, 12,
            13, 14, 15, 16
        );
        CHECK_EQUAL(1, mat.m11);
        CHECK_EQUAL(2, mat.m12);
        CHECK_EQUAL(3, mat.m13);
        CHECK_EQUAL(4, mat.m14);

        CHECK_EQUAL(5, mat.m21);
        CHECK_EQUAL(6, mat.m22);
        CHECK_EQUAL(7, mat.m23);
        CHECK_EQUAL(8, mat.m24);

        CHECK_EQUAL(9,  mat.m31);
        CHECK_EQUAL(10, mat.m32);
        CHECK_EQUAL(11, mat.m33);
        CHECK_EQUAL(12, mat.m34);

        CHECK_EQUAL(13, mat.m41);
        CHECK_EQUAL(14, mat.m42);
        CHECK_EQUAL(15, mat.m43);
        CHECK_EQUAL(16, mat.m44);
    }

    TEST(ScaleConstuctor)
    {
        auto mat = Woden::Math::Matrix4x4(2);
        CHECK_EQUAL(2, mat.m11);
        CHECK_EQUAL(0, mat.m12);
        CHECK_EQUAL(0, mat.m13);
        CHECK_EQUAL(0, mat.m14);

        CHECK_EQUAL(0, mat.m21);
        CHECK_EQUAL(2, mat.m22);
        CHECK_EQUAL(0, mat.m23);
        CHECK_EQUAL(0, mat.m24);

        CHECK_EQUAL(0, mat.m31);
        CHECK_EQUAL(0, mat.m32);
        CHECK_EQUAL(2, mat.m33);
        CHECK_EQUAL(0, mat.m34);

        CHECK_EQUAL(0, mat.m41);
        CHECK_EQUAL(0, mat.m42);
        CHECK_EQUAL(0, mat.m43);
        CHECK_EQUAL(2, mat.m44);
    }

    TEST(Inverse)
    {
        auto projectionMatrix = Woden::Math::Matrix4x4::ReverseDepthPerspective(60, 1, 0.1, 1000);
        auto inverseProjectionMatrix = projectionMatrix.inverse();
        auto identity = projectionMatrix * inverseProjectionMatrix;
        auto identity2 = inverseProjectionMatrix * projectionMatrix;

        CHECK(Woden::Math::closeTo(identity,  Woden::Math::Matrix4x4::Identity()));
        CHECK(Woden::Math::closeTo(identity2, Woden::Math::Matrix4x4::Identity()));
    }
}