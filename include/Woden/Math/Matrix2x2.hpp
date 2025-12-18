#ifndef WODEN_MATH_MATRIX2X2_HPP
#define WODEN_MATH_MATRIX2X2_HPP

#include "Vector2.hpp"

namespace Woden
{
namespace Math
{
/**
 * 2x2 Matrix
 */
class Matrix2x2
{
public:
    Matrix2x2(Scalar s = 1)
        : m11(s), m12(0),
          m21(0), m22(s)
    {
    }

    Matrix2x2(Scalar cm11, Scalar cm12, Scalar cm21, Scalar cm22)
        : m11(cm11), m12(cm12),
          m21(cm21), m22(cm22)
    {
    }

    Matrix2x2(Vector2 scale)
        : m11(scale.x), m12(0),
          m21(0), m22(scale.y)
    {
    }

    static Matrix2x2 Rotation(Scalar angle)
    {
        auto c = cos(angle);
        auto s = sin(angle);
        return Matrix2x2(
            c, -s,
            s, c
        );
    }

    static Matrix2x2 Identity()
    {
        return Matrix2x2(
            1, 0,
            0, 1
        );
    }

    static Matrix2x2 Ones()
    {
        return Matrix2x2(1, 1, 1, 1);
    }

    static Matrix2x2 Zeros()
    {
        return Matrix2x2(0, 0, 0, 0);
    }

    Scalar determinant() const
    {
        return m11*m22 - m21*m12;
    }

    Matrix2x2 inverse() const
    {
        Scalar det = determinant();
        return Matrix2x2(
            m22 / det, -m12 / det,
            -m21 / det, m11 / det
        );
    }

    Matrix2x2 operator+(const Matrix2x2 &o) const
    {
        return Matrix2x2(
            m11 + o.m11, m12 + o.m12,
            m21 + o.m21, m22 + o.m22
        );
    }

    Matrix2x2 operator-(const Matrix2x2 &o) const
    {
        return Matrix2x2(
            m11 - o.m11, m12 - o.m12,
            m21 - o.m21, m22 - o.m22
        );
    }

    Vector2 firstColumn() const
    {
        return Vector2(m11, m21);
    }

    Vector2 secondColumn() const
    {
        return Vector2(m12, m22);
    }

    Vector2 firstRow() const
    {
        return Vector2(m11, m12);
    }

    Vector2 secondRow() const
    {
        return Vector2(m21, m22);
    }

    friend Vector2 operator*(const Matrix2x2 &m, const Vector2 &v)
    {
        return m.firstColumn()*v.x + m.secondColumn()*v.y;
    }

    friend Vector2 operator*(const Vector2 &v, const Matrix2x2 &m)
    {
        return Vector2(
            m.firstColumn().dot(v),
            m.secondColumn().dot(v)
        );
    }

    Scalar m11, m12;
    Scalar m21, m22;
};

inline Matrix2x2 closeTo(Matrix2x2 a, Matrix2x2 b)
{
    return
        closeTo(a.m11, b.m11) && closeTo(a.m12, b.m12),
        closeTo(a.m21, b.m21) && closeTo(a.m22, b.m22);
}

}    
}

#endif // WODEN_MATH_MATRIX2X2_HPP
