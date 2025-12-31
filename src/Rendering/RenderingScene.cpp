#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/LightSource.hpp"

namespace Woden
{
namespace Rendering
{

void RenderingScene::addDirectionalLightSource(const DirectionalLightSource *lightSource)
{
}

void RenderingScene::addPointLightSource(const PointLightSource *lightSource)
{
    RenderingLightSourceObject renderingLight = {};
    renderingLight.castShadows = lightSource->castShadows;
    renderingLight.positionOrDirection = currentModelMatrix.fourthColumn();
    renderingLight.intensityAndColor = lightSource->color*lightSource->intensity;
    renderingLight.influenceRadius = lightSource->influenceRadius;
    lightSources.push_back(renderingLight);
    //printf("renderingLight.positionOrDirection %f %f %f %f\n",
    //    renderingLight.positionOrDirection.x, renderingLight.positionOrDirection.y, renderingLight.positionOrDirection.z, renderingLight.positionOrDirection.w
    //);
}

void RenderingScene::addSpotLightSource(const SpotLightSource *lightSource)
{
    RenderingLightSourceObject renderingLight = {};
    renderingLight.castShadows = lightSource->castShadows;
    renderingLight.positionOrDirection = currentModelMatrix.fourthColumn();
    renderingLight.spotDirection = currentModelMatrix.topLeftMatrix3x3().thirdColumn();
    renderingLight.intensityAndColor = lightSource->color*lightSource->intensity;
    renderingLight.influenceRadius = lightSource->influenceRadius;
    renderingLight.innerSpotCosCutoff = cos(lightSource->innerCutoff * (M_PI / 180.0));
    renderingLight.outerSpotCosCutoff = cos(lightSource->outerCutoff * (M_PI / 180.0));
    lightSources.push_back(renderingLight);
}

void RenderingScene::addObjectWithRenderable(const RenderablePtr &renderable)
{
    RenderingSceneObject sceneObject = {};
    sceneObject.renderable = renderable;
    sceneObject.modelMatrix = currentModelMatrix;
    sceneObject.inverseModelMatrix = currentInverseModelMatrix;
    opaqueObjects.push_back(sceneObject);
}

} // End of namespace Rendering
} // End of namespace Woden