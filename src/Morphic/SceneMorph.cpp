#include "Woden/Morphic/SceneMorph.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"

namespace Woden
{
namespace Morphic
{

SceneMorph::SceneMorph()
{
    color = Vector4(0, 0, 0, 1);
    setExtent(Vector2(1024, 576));

    sceneRenderer = std::make_shared<Rendering::SceneRenderer> ();
}

void SceneMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    Morph::drawWith(renderer);

    auto extent = getExtent();
    sceneRenderer->setupWithScreenSize(int(extent.x + 0.5), int(extent.y + 0.5));

    sceneRenderer->renderScene(scene);
}

} // End of namespace Morphic
} // End of namespace Woden