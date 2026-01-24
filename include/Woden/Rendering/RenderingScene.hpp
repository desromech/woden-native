#ifndef WODEN_RENDERING_SCENE_HPP
#define WODEN_RENDERING_SCENE_HPP

#include "Renderable.hpp"
#include "Camera.hpp"
#include "Woden/Math/TRSTransform3D.hpp"
#include "Woden/Math/Vector4.hpp"
#include <vector>

namespace Woden
{
namespace Rendering
{
class DirectionalLightSource;
class PointLightSource;
class SpotLightSource;

class RenderingSceneObject
{
public:
    uint32_t sceneObjectStateIndex;
    Math::Matrix4x4 modelMatrix;
    Math::Matrix4x4 inverseModelMatrix;
    RenderablePtr renderable;
};


struct ShadowMapAtlasAllocation
{
    Math::Vector2 offset;
    Math::Vector2 shadowMapExtent;
    Math::Vector2 shadowMapAtlasExtent;
};

class RenderingLightSourceObject
{
public:
    Math::Vector4 positionOrDirection;
    Math::Vector3 spotDirection;
    Math::Vector3 intensityAndColor;
    Math::Scalar influenceRadius = 1;
    Math::Scalar innerSpotCosCutoff = -1;
    Math::Scalar outerSpotCosCutoff = -1;
    bool castShadows = false;

    float shadowMapNormalBiasFactor;
	Math::Vector4 shadowMapCascadeDistanceWorldTransform;
	Math::Vector4 shadowMapCascadeOffsets;

    uint32_t shadowMapPartCount = 0;
    Math::Matrix4x4 modelMatrix[6];
	Math::Matrix4x4 inverseModelMatrix[6];

	Math::Matrix4x4 projectionMatrix[6];
	Math::Matrix4x4 inverseProjectionMatrix[6];
    
    Math::Vector2 shadowMapViewportOffsets;
    ShadowMapAtlasAllocation atlasAllocations[6];
};

class ShadowMapAtlasAllocator
{
public:
    void initializeWithExtent(uint32_t atlasWidth, uint32_t atlasHeight);
    void reset();
    bool allocate(ShadowMapAtlasAllocation *outAllocation);
    
    uint32_t atlasWidth;
    uint32_t atlasHeight;

    uint32_t columns;
    uint32_t rows;
    uint32_t capacity;
    uint32_t size;
    Math::Vector2 shadowMapExtent;
};

class RenderingScene
{
public:
    template<typename FT>
    void withTRSTransformDuring(const Woden::Math::TRSTransform3D &transform, FT &&aBlock)
    {
        auto oldModelMatrix = currentModelMatrix;
        auto oldInverseModelMatrix = currentInverseModelMatrix;

        currentModelMatrix = currentModelMatrix * transform.asMatrix();
        currentInverseModelMatrix = transform.asInverseMatrix() * currentInverseModelMatrix;

        aBlock();
        
        currentModelMatrix = oldModelMatrix;
        currentInverseModelMatrix = oldInverseModelMatrix;
    }

    void addDirectionalLightSource(const DirectionalLightSource *lightSource);
    void addPointLightSource(const PointLightSource *lightSource);
    void addSpotLightSource(const SpotLightSource *lightSource);

    void addObjectWithRenderable(const RenderablePtr &renderable);
    void addTranslucentObjectWithRenderable(const RenderablePtr &renderable);

    Math::Matrix4x4 currentModelMatrix;
    Math::Matrix4x4 currentInverseModelMatrix;

    Math::Matrix4x4 currentViewMatrix;
    Math::Matrix4x4 currentInverseViewMatrix;

    Math::Frustum currentViewFrustum;
    Math::Frustum currentWorldFrustum;

    CameraPtr currentCamera;

    std::vector<RenderingSceneObject> backgroundObjects;
    std::vector<RenderingSceneObject> opaqueObjects;
    std::vector<RenderingSceneObject> translucentObject;
    std::vector<RenderingLightSourceObject> lightSources;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERING_ELEMENT_HPP