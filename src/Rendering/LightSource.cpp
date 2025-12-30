#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/RenderingScene.hpp"

namespace Woden
{
namespace Rendering
{

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