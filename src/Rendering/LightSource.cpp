#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace Rendering
{

SceneGraph::SceneNodePtr LightSource::asSceneNodeWithPosition(const Math::Vector3 &position)
{
    auto node = std::make_shared<SceneGraph::SceneNode> ();
    node->addLightSource(shared_from_this());
    node->transform.translation = position;
    return node;
}

SceneGraph::SceneNodePtr LightSource::asSceneNode()
{
    return asSceneNodeWithPosition(Math::Vector3(0));
}

void DirectionalLightSource::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    renderingScene->addDirectionalLightSource(this);
}

void PointLightSource::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    renderingScene->addPointLightSource(this);
}

void SpotLightSource::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    renderingScene->addSpotLightSource(this);
}

} // End of namespace Rendering
} // End of namespace Woden