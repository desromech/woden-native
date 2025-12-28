#include "Woden/Rendering/StaticMeshRenderable.hpp"
#include "Woden/Rendering/MeshPrimitive.hpp"

namespace Woden
{
namespace Rendering
{

void StaticMeshRenderable::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    for(auto &primitive : primitives)
        primitive->addIntoRenderingScene(renderingScene);
}

} // End of namespace Rendering
} // End of namespace Woden