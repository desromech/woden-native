#ifndef WODEN_SCENE_RENDERER_HPP
#define WODEN_SCENE_RENDERER_HPP

#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Assets/BinaryBuffer.hpp"
#include "AGPU/agpu.hpp"

namespace Woden
{
namespace Rendering
{

class SceneRendererScreen
{
public:
    agpu_texture_ref depthStencilBuffer;
    agpu_texture_view_ref depthStencilAttachmentView;
    agpu_texture_view_ref depthStencilSampledView;

    agpu_texture_ref hdrColorBuffer;
    agpu_texture_view_ref hdrColorBufferView;

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
    void renderScene(const agpu_command_list_ref &commandList, const SceneGraph::ScenePtr &scene);
    void setupWithScreenSize(int newScreenWidth, int newScreenHeight);

    void useIndexBuffer(const Assets::BinaryBufferAccessorPtr &indices);

    std::shared_ptr<SceneRendererScreen> screen;
    agpu_command_list_ref currentCommandList;
    RenderingScenePtr currentRenderingScene;

private:
    void performDepthOnlyPass();
    void performHDROpaquePass();

};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERER_HPP
