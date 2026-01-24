#ifndef WODEN_RENDERING_CAMERA_HPP
#define WODEN_RENDERING_CAMERA_HPP

#include "Woden/Math/Matrix4x4.hpp"
#include "Woden/Math/Frustum.hpp"

#include <memory.h>

namespace Woden
{
namespace Rendering
{
typedef std::shared_ptr<class Camera> CameraPtr;

class Camera
{
public:
    Math::Matrix4x4 computeProjectionMatrix(Math::Scalar aspect)
    {
        if(isPerspective)
            return Math::Matrix4x4::ReverseDepthPerspective(fovY, aspect, nearDistance, farDistance);
        
        auto hh = tan((fovY *0.5 ) *M_PI / 180) * focalDistance;
		auto hw = hh * aspect;

        return Math::Matrix4x4::ReverseDepthOrtho(-hw, hw, -hh, hh, orthographicNearDistance, orthographicFarDistance);
    }

    Math::Frustum computeViewFrustum(Math::Scalar aspect)
    {
        if(isPerspective)
            return Math::Frustum::MakePerspective(fovY, aspect, nearDistance, farDistance);

        // FIXME: Use an orthographic frustum
        auto hh = tan((fovY *0.5 ) *M_PI / 180) * focalDistance;
		auto hw = hh * aspect;
        return Math::Frustum::MakeOrtho(-hw, hw, -hh, hh, orthographicNearDistance, orthographicFarDistance);
    }

    bool isPerspective = true;
    Math::Scalar nearDistance = 0.1f;
    Math::Scalar farDistance = 1000.0f;
    Math::Scalar orthographicNearDistance = -1000.0f;
    Math::Scalar orthographicFarDistance = 1000.0f;

    Math::Scalar fovY = 60.0;
    Math::Scalar focalDistance = 10.0;

    Math::Scalar cascadeSplitDistributionLambda = 0.99f;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_CAMERA_HPP
