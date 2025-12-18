#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Matrix2x2.hpp"

SUITE(Matrix2x2)
{
    TEST(Constructor)
    {
        auto mat = Woden::Math::Matrix2x2(1, 2, 3, 4);
        CHECK_EQUAL(1, mat.m11);
        CHECK_EQUAL(2, mat.m12);

        CHECK_EQUAL(3, mat.m21);
        CHECK_EQUAL(4, mat.m22);
    }

    TEST(ScaleConstuctor)
    {
        auto mat = Woden::Math::Matrix2x2(2);
        CHECK_EQUAL(2, mat.m11);
        CHECK_EQUAL(0, mat.m12);

        CHECK_EQUAL(0, mat.m21);
        CHECK_EQUAL(2, mat.m22);
    }

    TEST(ScaleConstuctor2)
    {
        auto mat = Woden::Math::Matrix2x2(Woden::Math::Vector2(1, 2));
        CHECK_EQUAL(1, mat.m11);
        CHECK_EQUAL(0, mat.m12);

        CHECK_EQUAL(0, mat.m21);
        CHECK_EQUAL(2, mat.m22);
    }

    TEST(Determinant)
    {
        CHECK_EQUAL(0, Woden::Math::Matrix2x2::Ones().determinant());
        CHECK_EQUAL(0, Woden::Math::Matrix2x2::Zeros().determinant());
        CHECK_EQUAL(1, Woden::Math::Matrix2x2::Identity().determinant());
        CHECK_EQUAL(-2, Woden::Math::Matrix2x2(1, 3, 2, 4).determinant());
        CHECK_EQUAL(-2, Woden::Math::Matrix2x2(1, 2, 3, 4).determinant());
    }

}