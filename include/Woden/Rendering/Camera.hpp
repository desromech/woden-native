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
        return Math::Matrix4x4::ReverseDepthPerspective(fovY, aspect, nearDistance, farDistance);
    }

    Math::Frustum computeViewFrustum(Math::Scalar aspect)
    {
        return Math::Frustum::MakePerspective(fovY, aspect, nearDistance, farDistance);
    }

    bool isPerspective = true;
    Math::Scalar nearDistance = 0.1f;
    Math::Scalar farDistance = 1000.0;
    Math::Scalar fovY = 60.0;

    Math::Scalar cascadeSplitDistributionLambda = 0.99f;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_CAMERA_HPP
