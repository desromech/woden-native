#ifndef WODEN_SCENE_RENDERER_HPP
#define WODEN_SCENE_RENDERER_HPP

#include "Woden/SceneGraph/Scene.hpp"
#include "AGPU/agpu.hpp"

namespace Woden
{
namespace Rendering
{

/**
 * Scene renderer
 */
class SceneRenderer
{
public:
    void renderScene(SceneGraph::ScenePtr scene);
    void setupWithScreenSize(int newScreenWidth, int newScreenHeight);

    int screenWidth = -1;
    int screenHeight = -1;

    agpu_texture_ref depthStencilBuffer;
    agpu_texture_ref hdrColorBuffer;
    agpu_framebuffer_ref depthOnlyFramebuffer;
    agpu_framebuffer_ref hdrOpaqueFramebuffer;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERER_HPP
