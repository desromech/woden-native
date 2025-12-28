#ifndef WODEN_SCENE_RENDERER_HPP
#define WODEN_SCENE_RENDERER_HPP

#include "Woden/SceneGraph/Scene.hpp"
#include "AGPU/agpu.hpp"

namespace Woden
{
namespace Rendering
{

class SceneRendererScreen
{
public:
    agpu_texture_ref depthStencilBuffer;
    agpu_texture_ref hdrColorBuffer;
    agpu_framebuffer_ref depthOnlyFramebuffer;
    agpu_framebuffer_ref hdrOpaqueFramebuffer;

    int screenWidth = -1;
    int screenHeight = -1;
};

/**
 * Scene renderer
 */
class SceneRenderer
{
public:
    static const agpu_texture_format HDRColorBufferFormat = AGPU_TEXTURE_FORMAT_R16G16B16A16_FLOAT;
    static const agpu_texture_format DepthStencilBufferFormat = AGPU_TEXTURE_FORMAT_R32_TYPELESS;
    static const agpu_texture_format DepthStencilBufferViewFormat = AGPU_TEXTURE_FORMAT_D32_FLOAT;

    void renderScene(const SceneGraph::ScenePtr &scene);
    void setupWithScreenSize(int newScreenWidth, int newScreenHeight);

    std::shared_ptr<SceneRendererScreen> screen;

};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERER_HPP
