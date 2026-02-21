#ifndef WODEN_MATH_RIGID_TRANSFORM_HPP
#define WODEN_MATH_RIGID_TRANSFORM_HPP

#include "Quaternion.hpp"
#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include "TRSTransform3D.hpp"

namespace Woden
{
namespace Math
{

/**
 *I am a 3D transform that is decomposed in a successive sequence of scale, rotation, and translation.
 */
struct RigidTransform
{

    static RigidTransform Identity()
    {
        return RigidTransform();
    }

    Vector3 transformPosition(const Vector3 &position) const
    {
        return rotation.rotateVector(position) + translation;
    }

    Vector3 transformNormalVector(const Vector3 &normal) const
    {
        return rotation.rotateVector(normal);
    }

    Vector3 inverseTransformPosition(const Vector3 &position) const
    {
        return rotation.conjugated().rotateVector(position - translation);
    }

    Vector3 inverseTransformNormalVector(const Vector3 &normal) const
    {
        return rotation.conjugated().rotateVector(normal);
    }

    Matrix3x3 asMatrix3x3() const
    {
        return rotation.asMatrix();
    }

    Matrix4x4 asMatrix() const
    {
        return Matrix4x4::WithMatrix3x3AndTranslation(asMatrix3x3(), translation);
    }

    Matrix3x3 asInverseMatrix3x3() const
    {
        return rotation.conjugated().asMatrix();
    }

    Vector3 inverseTranslation() const
    {
        return rotation.inverseRotateVector(-translation);
    }

    Matrix4x4 asInverseMatrix() const
    {
        return Matrix4x4::WithMatrix3x3AndTranslation(asInverseMatrix3x3(), inverseTranslation());
    }

    TRSTransform3D asTRSTransform3D() const
    {
        TRSTransform3D transform;
        transform.rotation = rotation;
        transform.translation = translation;
        return transform;
    }

    bool operator==(const RigidTransform &o) const
    {
        return rotation == o.rotation && translation == o.translation;
    }

    Quaternion rotation = Quaternion::Identity();
    Vector3 translation = Vector3(0, 0, 0);
};

} // namespace Math
} // namespace Woden

#endif // WODEN_MATH_RIGID_TRANSFORM_HPP