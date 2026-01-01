#ifndef WODEN_MATH_FRUSTUM_HPP
#define WODEN_MATH_FRUSTUM_HPP

#include "Vector3.hpp"
#include "Matrix4x4.hpp"
#include "Plane.hpp"

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

    static Frustum MakePerspective(Scalar fovy, Scalar aspect, Scalar near, Scalar far)
    {
        auto fovyRad = fovy *(M_PI/180.0) * 0.5;
        auto top = near * tan(fovyRad);
        auto right = top * aspect;
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
};

} // End of namespace Math
} // End of namespace Woden

#endif //WODEN_MATH_FRUSTUM_HPP