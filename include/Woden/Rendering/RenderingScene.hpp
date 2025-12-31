#ifndef WODEN_RENDERING_SCENE_HPP
#define WODEN_RENDERING_SCENE_HPP

#include "Renderable.hpp"
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

    Math::Matrix4x4 currentModelMatrix;
    Math::Matrix4x4 currentInverseModelMatrix;

    Math::Matrix4x4 currentViewMatrix;
    Math::Matrix4x4 currentInverseViewMatrix;

    std::vector<RenderingSceneObject> backgroundObjects;
    std::vector<RenderingSceneObject> opaqueObjects;
    std::vector<RenderingSceneObject> translucentObject;
    std::vector<RenderingLightSourceObject> lightSources;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERING_ELEMENT_HPP