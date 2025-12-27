#include "Woden/Morphic/SceneMorph.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

namespace Woden
{
namespace Morphic
{

SceneMorph::SceneMorph()
{
    color = Vector4(0, 0, 0, 1);
    setExtent(Vector2(1024, 576));
}

void SceneMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    Morph::drawWith(renderer);
}

} // End of namespace Morphic
} // End of namespace Woden