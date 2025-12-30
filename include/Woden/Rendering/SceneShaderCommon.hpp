#ifndef WODEN_SCENE_SHADER_COMMON_HPP
#define WODEN_SCENE_SHADER_COMMON_HPP

#include "Woden/Math/Matrix4x4.hpp"
#include "Woden/Math/Vector4.hpp"

namespace Woden
{
namespace Rendering
{

struct SceneObjectState
{
    Math::Matrix4x4 transformationMatrix;
    Math::Matrix4x4 inverseTransformationMatrix;
};

struct SceneCameraState
{
    Math::Matrix4x4 transformationMatrix;
    Math::Matrix4x4 inverseTransformationMatrix;

    Math::Matrix4x4 projectionMatrix;
    Math::Matrix4x4 inverseProjectionMatrix;
};

struct GlobalLightingState
{
    Math::CompactVector3 ambientLighting = Math::CompactVector3(0.2, 0.2, 0.2);
    uint32_t numberOfLights;
};

struct LightSourceState
{
    Math::Vector4 positionOrDirection = Math::Vector4(0, 0, 0, 0);

    Math::CompactVector3 intensity = Math::CompactVector3(0, 0, 0);
    float radius = 0.0;
};

struct ScenePushConstants
{
    uint32_t objectStateIndex = 0;
    uint32_t cameraStateIndex = 0;
    uint32_t materialStateIndex = 0;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_SHADER_COMMON_HPP
