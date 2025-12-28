#ifndef WODEN_RENDERING_SCENE_HPP
#define WODEN_RENDERING_SCENE_HPP

#include "Renderable.hpp"
#include <vector>

namespace Woden
{
namespace Rendering
{

class RenderingSceneObject
{
public:
    RenderablePtr renderable;
};

class RenderingScene
{
public:
    void addObjectWithRenderable(const RenderablePtr &renderable);

    std::vector<RenderingSceneObject> backgroundObjects;
    std::vector<RenderingSceneObject> opaqueObjects;
    std::vector<RenderingSceneObject> translucentObject;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERING_ELEMENT_HPP