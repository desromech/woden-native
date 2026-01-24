#ifndef WODEN_MATH_FRUSTUM_HPP
#define WODEN_MATH_FRUSTUM_HPP

#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include "Plane.hpp"
#include "Ray3D.hpp"

namespace Woden
{
namespace Math
{

/**
 * I am a frustum. I am typically used for camera viewport culling.
 */
struct Frustum
{

    Vector3 leftBottomNear;
    Vector3 rightBottomNear;
    Vector3 leftTopNear;
    Vector3 rightTopNear;
    Vector3 leftBottomFar;
    Vector3 rightBottomFar;
    Vector3 leftTopFar;
    Vector3 rightTopFar;

    Plane planes[6];

    static Frustum MakeFrustum(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar near, Scalar far)
    {
        Frustum result = {};

        result.leftBottomNear  = Vector3(left, bottom,  -near);
        result.rightBottomNear = Vector3(right, bottom, -near);
        result.leftTopNear     = Vector3(left, top,     -near);
        result.rightTopNear    = Vector3(right, top,    -near);

        auto factor = far / near;
        result.leftBottomFar  = result.leftBottomNear  * factor;
        result.rightBottomFar = result.rightBottomNear * factor;
        result.leftTopFar     = result.leftTopNear     * factor;
        result.rightTopFar    = result.rightTopNear    * factor;

        result.computePlanes();
        return result;
    }

    static Frustum MakeOrtho(Scalar left, Scalar right, Scalar bottom, Scalar top, Scalar near, Scalar far)
    {
        Frustum result = {};

        result.leftBottomNear  = Vector3(left, bottom,  near);
        result.rightBottomNear = Vector3(right, bottom, near);
        result.leftTopNear     = Vector3(left, top,     near);
        result.rightTopNear    = Vector3(right, top,    near);

        result.leftBottomFar  = Vector3(left, bottom,  far);
        result.rightBottomFar = Vector3(right, bottom, far);
        result.leftTopFar     = Vector3(left, top,     far);
        result.rightTopFar    = Vector3(right, top,    far);

        result.computePlanes();
        return result;
    }

    static Frustum MakePerspective(Scalar fovy, Scalar aspect, Scalar near, Scalar far)
    {
        Scalar fovyRad = Scalar(fovy *(M_PI/180.0) * 0.5);
        Scalar top = Scalar(near * tan(fovyRad));
        Scalar right = top * aspect;
        return MakeFrustum(-right, right, -top, top, near, far);
    }

    void computePlanes()
    {
		// Near plane
   		planes[0] = Plane::WithPoints(leftBottomNear, rightBottomNear, leftTopNear);
		// Far plane
   		planes[1] = Plane::WithPoints(rightBottomFar, leftBottomFar, leftTopFar);

		// Left
		planes[2] = Plane::WithPoints(leftBottomNear, leftTopNear, leftTopFar);
		// Right
		planes[3] = Plane::WithPoints(rightTopNear, rightBottomNear, rightTopFar);

        // Bottom
   		planes[4] = Plane::WithPoints(rightBottomNear, leftBottomNear, leftBottomFar);
		// Top
		planes[5] = Plane::WithPoints(leftTopNear, rightTopNear,leftTopFar);
    }

    template<typename FT>
    void cornersDo(FT&& f)
    {
        f(leftBottomNear);
        f(rightBottomNear);
        f(leftTopNear);
        f(rightTopNear);
        f(leftBottomFar);
        f(rightBottomFar);
        f(leftTopFar);
        f(rightTopFar);
    }

    Frustum transformedWithMatrix(const Matrix4x4 &m)
    {
        Frustum result;
        result.leftBottomNear  = m.transformPosition(leftBottomNear);
        result.rightBottomNear = m.transformPosition(rightBottomNear);
        result.leftTopNear     = m.transformPosition(leftTopNear);
        result.rightTopNear    = m.transformPosition(rightTopNear);
        result.leftBottomFar   = m.transformPosition(leftBottomFar);
        result.rightBottomFar  = m.transformPosition(rightBottomFar);
        result.leftTopFar      = m.transformPosition(leftTopFar);
        result.rightTopFar     = m.transformPosition(rightTopFar);
        result.computePlanes();
        return result;
    }

    Frustum splitAtLambdas(Scalar nearLambda, Scalar farLambda)
    {
        Frustum result;
        result.leftBottomNear  = mix(leftBottomNear,  leftBottomFar,  nearLambda);
        result.rightBottomNear = mix(rightBottomNear, rightBottomFar, nearLambda);
        result.leftTopNear     = mix(leftTopNear,     leftTopFar,     nearLambda);
        result.rightTopNear    = mix(rightTopNear,    rightTopFar,    nearLambda);

        result.leftBottomFar  = mix(leftBottomNear,  leftBottomFar,  farLambda);
        result.rightBottomFar = mix(rightBottomNear, rightBottomFar, farLambda);
        result.leftTopFar     = mix(leftTopNear,     leftTopFar,     farLambda);
        result.rightTopFar    = mix(rightTopNear,    rightTopFar,    farLambda);
        result.computePlanes();
        return result;
    }

    Vector3 normalizedPointInNearPlane(const Vector2 &normalizedPoint) const
    {
        auto bottom = mix(leftBottomNear, rightBottomNear, normalizedPoint.x);
        auto top = mix(leftTopNear, rightTopNear, normalizedPoint.x);
        return mix(bottom, top, normalizedPoint.y);
    }

    Vector3 normalizedPointInFarPlane(const Vector2 &normalizedPoint) const
    {
        auto bottom = mix(leftBottomFar, rightBottomFar, normalizedPoint.x);
        auto top = mix(leftTopFar, rightTopFar, normalizedPoint.x);
        return mix(bottom, top, normalizedPoint.y);
    }

    Ray3D rayForNormalizedPoint(const Vector2 &normalizedPoint) const
    {
        auto nearPoint = normalizedPointInNearPlane(normalizedPoint);
        auto farPoint = normalizedPointInFarPlane(normalizedPoint);
        return Ray3D::FromTo(nearPoint, farPoint);
    }
};

} // End of namespace Math
} // End of namespace Woden

#endif //WODEN_MATH_FRUSTUM_HPP