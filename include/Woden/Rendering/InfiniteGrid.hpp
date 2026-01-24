#ifndef WODEN_RENDERING_INFINITE_GRID_HPP
#define WODEN_RENDERING_INFINITE_GRID_HPP

#include "Renderable.hpp"

namespace Woden
{
namespace Rendering
{

class InfiniteGridRenderable : public Renderable
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) override;
    virtual void renderTranslucentWith(SceneRenderer *renderer) override;

};

} // End of namespace Rendering
} // End of namespace Woden
#endif //WODEN_RENDERING_INFINITE_GRID_HPP