#ifndef WODEN_SCENE_SHADER_COMMON_HPP
#define WODEN_SCENE_SHADER_COMMON_HPP

#include "Woden/Math/Matrix4x4.hpp"
#include "Woden/Math/Vector2.hpp"

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
    Math::Vector2 framebufferExtent;
    Math::Vector2 framebufferReciprocalExtent;

    uint32_t flipVertically;
    float nearDistance = 0;
    float farDistance = 100.0;
    float exposure = 1;
    
    Math::Vector2 lightGridDepthSliceScaleOffset;
	uint32_t hasTopLeftNDCOrigin;
	uint32_t hasBottomLeftTextureCoordinates;

    uint32_t lightGridWidth;
    uint32_t lightGridHeight;
    uint32_t lightGridDepth;
    float padding = 0;

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
    float influenceRadius = 0.0;
};

struct LightCluster
{
    Math::CompactVector3 min;
    Math::Scalar padding;

    Math::CompactVector3 max;
    Math::Scalar padding2;
};

struct SceneSurfaceMaterial
{
    float alphaCutoff = 0.5;
    uint32_t doubleSided = false;
    Math::Vector2 padding;
};

struct SceneMetallicRoughnessMaterial : SceneSurfaceMaterial
{
    Math::Vector4 baseColorFactor = Math::Vector4(1, 1, 1, 1);

    Math::CompactVector3 emissiveFactor = Math::CompactVector3(0, 0, 0);
    float occlusionFactor = 1;

    float roughnessFactor = 1;
    float metallicFactor = 1;
    Math::Vector2 texcoordOffset = Math::Vector2(0, 0);

    Math::Vector2 texcoordScale = Math::Vector2(1, 1);
    Math::Vector2 texcoordOffsetVelocity = Math::Vector2(0, 0);
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
