#include "Woden/Rendering/MeshPrimitive.hpp"
#include "Woden/Rendering/Material.hpp"
#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/VertexBinding.hpp"

namespace Woden
{
namespace Rendering
{

void MeshPrimitive::addIntoRenderingScene(const RenderingScenePtr &renderingScene)
{
    renderingScene->addObjectWithRenderable(shared_from_this());
}

MaterialPtr MeshPrimitive::getValidMaterial() const
{
    return material ? material : Material::getDefaultMaterial();
}

void MeshPrimitive::renderDepthOnlyWith(SceneRenderer *renderer) 
{
    if(!getValidMaterial()->activateDepthOnlyWithRenderer(renderer))
        return;

    auto &commandList = renderer->currentCommandList;
    vertexBinding->useWithRenderer(renderer);
    renderer->useIndexBuffer(indices);
    commandList->drawElements(indices->count, 1, 0, 0, 0);
}

void MeshPrimitive::renderOpaqueWith(SceneRenderer *renderer) 
{
    if(!getValidMaterial()->activateOpaqueWithRenderer(renderer))
        return;

    auto &commandList = renderer->currentCommandList;
    vertexBinding->useWithRenderer(renderer);
    renderer->useIndexBuffer(indices);
    commandList->drawElements(indices->count, 1, 0, 0, 0);
}

} // End of namespace Rendering
} // End of namespace Woden