#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/RenderingScene.hpp"

namespace Woden
{
namespace Rendering
{

void LightSource::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    renderingScene->addLightSource(this);
}

} // End of namespace Rendering
} // End of namespace Woden