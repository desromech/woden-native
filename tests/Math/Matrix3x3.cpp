#include "UnitTest++/UnitTest++.h"
#include "Woden/Math/Matrix3x3.hpp"

SUITE(Matrix3x3)
{
    TEST(Constructor)
    {
        auto mat = Woden::Math::Matrix3x3(1, 2, 3, 4, 5, 6, 7, 8, 9);
        CHECK_EQUAL(1, mat.m11);
        CHECK_EQUAL(2, mat.m12);
        CHECK_EQUAL(3, mat.m13);

        CHECK_EQUAL(4, mat.m21);
        CHECK_EQUAL(5, mat.m22);
        CHECK_EQUAL(6, mat.m23);

        CHECK_EQUAL(7, mat.m31);
        CHECK_EQUAL(8, mat.m32);
        CHECK_EQUAL(9, mat.m33);
    }

    TEST(ScaleConstuctor)
    {
        auto mat = Woden::Math::Matrix3x3(2);
        CHECK_EQUAL(2, mat.m11);
        CHECK_EQUAL(0, mat.m12);
        CHECK_EQUAL(0, mat.m13);

        CHECK_EQUAL(0, mat.m21);
        CHECK_EQUAL(2, mat.m22);
        CHECK_EQUAL(0, mat.m23);

        CHECK_EQUAL(0, mat.m31);
        CHECK_EQUAL(0, mat.m32);
        CHECK_EQUAL(2, mat.m33);
    }

    TEST(ScaleConstuctor2)
    {
        auto mat = Woden::Math::Matrix3x3(Woden::Math::Vector3(1, 2, 3));
        CHECK_EQUAL(1, mat.m11);
        CHECK_EQUAL(0, mat.m12);
        CHECK_EQUAL(0, mat.m13);

        CHECK_EQUAL(0, mat.m21);
        CHECK_EQUAL(2, mat.m22);
        CHECK_EQUAL(0, mat.m23);

        CHECK_EQUAL(0, mat.m31);
        CHECK_EQUAL(0, mat.m32);
        CHECK_EQUAL(3, mat.m33);
    }

    TEST(Determinant)
    {
        CHECK_EQUAL(0, Woden::Math::Matrix3x3::Ones().determinant());
        CHECK_EQUAL(0, Woden::Math::Matrix3x3::Zeros().determinant());
        CHECK_EQUAL(1, Woden::Math::Matrix3x3::Identity().determinant());
        CHECK_EQUAL(0, Woden::Math::Matrix3x3::RowMajorIndices().determinant());
        CHECK_EQUAL(160, Woden::Math::Matrix3x3(
            0, 1, 2,
            10, 15, 20,
            -10, 7, 8
        ).determinant());
    }

    TEST(Transpose)
    {
        CHECK_EQUAL(Woden::Math::Matrix3x3::ColumnMajorIndices(), Woden::Math::Matrix3x3::ColumnMajorIndices().transpose().transpose());
        CHECK_EQUAL(Woden::Math::Matrix3x3::RowMajorIndices(), Woden::Math::Matrix3x3::ColumnMajorIndices().transpose());
    }   

    TEST(Identity)
    {
        CHECK_EQUAL(Woden::Math::Vector3(1, 2, 3), Woden::Math::Matrix3x3::Identity() * Woden::Math::Vector3(1, 2, 3));
        CHECK_EQUAL(Woden::Math::Vector3(1, 2, 3), Woden::Math::Vector3(1, 2, 3) * Woden::Math::Matrix3x3::Identity());
    }

    TEST(Inverse)
    {
        CHECK_EQUAL(Woden::Math::Matrix3x3::Identity(), Woden::Math::Matrix3x3::Identity().inverse());

        auto m = Woden::Math::Matrix3x3(0, 1, 2, 3, -4, 5, -6, 7, 8);
        auto im = m.inverse();
        CHECK(Woden::Math::closeTo(Woden::Math::Matrix3x3::Identity(), m*im));
        CHECK(Woden::Math::closeTo(Woden::Math::Matrix3x3::Identity(), im*m));
    }

    TEST(Scale)
    {
        CHECK_EQUAL(Woden::Math::Vector3(2, 4, 6), Woden::Math::Matrix3x3(2) * Woden::Math::Vector3(1, 2, 3));
        CHECK_EQUAL(Woden::Math::Vector3(2, 4, 6), Woden::Math::Vector3(1, 2, 3) * Woden::Math::Matrix3x3(2));
    }

    TEST(RowAndColumnMajor)
    {
        CHECK_EQUAL(Woden::Math::Matrix3x3(
            5, 14, 23,
            14, 50, 86,
            23, 86, 149
        ), Woden::Math::Matrix3x3::RowMajorIndices()*Woden::Math::Matrix3x3::ColumnMajorIndices());
        CHECK_EQUAL(Woden::Math::Matrix3x3(
            45, 54, 63,
	        54, 66, 78,
	        63, 78, 93
        ), Woden::Math::Matrix3x3::ColumnMajorIndices()*Woden::Math::Matrix3x3::RowMajorIndices());
    }
}