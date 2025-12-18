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

    TEST(Transpose)
    {
        CHECK_EQUAL(Woden::Math::Matrix2x2::ColumnMajorIndices(), Woden::Math::Matrix2x2::ColumnMajorIndices().transpose().transpose());
        CHECK_EQUAL(Woden::Math::Matrix2x2::RowMajorIndices(), Woden::Math::Matrix2x2::ColumnMajorIndices().transpose());
    }   

    TEST(Identity)
    {
        CHECK_EQUAL(Woden::Math::Vector2(1, 2), Woden::Math::Matrix2x2::Identity() * Woden::Math::Vector2(1, 2));
        CHECK_EQUAL(Woden::Math::Vector2(1, 2), Woden::Math::Vector2(1, 2) * Woden::Math::Matrix2x2::Identity());
    }

    TEST(Inverse)
    {
        CHECK_EQUAL(Woden::Math::Matrix2x2::Identity(), Woden::Math::Matrix2x2::Identity().inverse());
        CHECK_EQUAL(Woden::Math::Matrix2x2(-1.5, 0.5, 1, 0), Woden::Math::Matrix2x2(0, 1, 2, 3).inverse());
        CHECK_EQUAL(Woden::Math::Matrix2x2::Identity(), Woden::Math::Matrix2x2(-1.5, 0.5, 1, 0)*Woden::Math::Matrix2x2(0, 1, 2, 3));
        CHECK_EQUAL(Woden::Math::Matrix2x2::Identity(), Woden::Math::Matrix2x2(0, 1, 2, 3)*Woden::Math::Matrix2x2(-1.5, 0.5, 1, 0));
    }

    TEST(Scale)
    {
        CHECK_EQUAL(Woden::Math::Vector2(2, 4), Woden::Math::Matrix2x2(2) * Woden::Math::Vector2(1, 2));
        CHECK_EQUAL(Woden::Math::Vector2(2, 4), Woden::Math::Vector2(1, 2) * Woden::Math::Matrix2x2(2));
    }   

    TEST(RowAndColumnMajor)
    {
        CHECK_EQUAL(Woden::Math::Matrix2x2(1, 3, 3, 13), Woden::Math::Matrix2x2::RowMajorIndices()*Woden::Math::Matrix2x2::ColumnMajorIndices());
        CHECK_EQUAL(Woden::Math::Matrix2x2(4, 6, 6, 10), Woden::Math::Matrix2x2::ColumnMajorIndices()*Woden::Math::Matrix2x2::RowMajorIndices());
    } 
}