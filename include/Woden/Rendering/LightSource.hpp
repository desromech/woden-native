#ifndef WODEN_RENDERING_LIGHT_SOURCE
#define WODEN_RENDERING_LIGHT_SOURCE

#include "Woden/Math/Vector3.hpp"
#include <memory>

namespace Woden
{

namespace SceneGraph
{
typedef std::shared_ptr<class SceneNode> SceneNodePtr;

} // End of namespace SceneGraph

namespace Rendering
{

typedef std::shared_ptr<class RenderingScene> RenderingScenePtr;

typedef std::shared_ptr<class LightSource> LightSourcePtr;
class LightSource : public std::enable_shared_from_this<LightSource>
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) = 0;

    SceneGraph::SceneNodePtr asSceneNodeWithPosition(const Math::Vector3 &position);
    SceneGraph::SceneNodePtr asSceneNode();

    Math::Vector3 color = Math::Vector3(1, 1, 1);
    Math::Scalar intensity = 1;

    bool castShadows = false;

	Math::Scalar shadowMapDepthBiasConstant = 2.0;
	Math::Scalar shadowMapDepthBiasSlope = 1.0;
	Math::Scalar shadowMapNormalBiasFactor = 0.005;
};

class DirectionalLightSource : public LightSource
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) override;
	
    Math::Scalar shadowCastingRadius = 1000.0;
};

class PointLightSource : public LightSource
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) override;

    Math::Scalar influenceRadius;
};

class SpotLightSource : public PointLightSource
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) override;

    Math::Scalar innerCutoff = 45;
    Math::Scalar outerCutoff = 60;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif // WODEN_RENDERING_LIGHT_SOURCE