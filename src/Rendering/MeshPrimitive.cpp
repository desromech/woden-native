#include "Woden/Rendering/MeshPrimitive.hpp"
#include "Woden/Rendering/RenderingScene.hpp"

namespace Woden
{
namespace Rendering
{

void MeshPrimitive::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    renderingScene->addObjectWithRenderable(shared_from_this());
}

} // End of namespace Rendering
} // End of namespace Woden