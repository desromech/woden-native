#include "Woden/Rendering/SceneRenderer.hpp"

namespace Woden
{
namespace Rendering
{

void SceneRenderer::renderScene(SceneGraph::ScenePtr scene)
{
}

void SceneRenderer::setupWithScreenSize(int newScreenWidth, int newScreenHeight)
{
    if(screenHeight == newScreenHeight && screenWidth == newScreenWidth && hdrOpaqueFramebuffer)
        return;
    screenHeight = newScreenHeight;
    screenWidth = newScreenWidth;
}

} // End of namespace Rendering
} // End of namespace Woden