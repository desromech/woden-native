#ifndef WODEN_SCENE_RENDERER_HPP
#define WODEN_SCENE_RENDERER_HPP

#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Assets/BinaryBuffer.hpp"
#include "Woden/Rendering/SceneShaderCommon.hpp"

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

    agpu_texture_ref normalGBuffer;
    agpu_texture_view_ref normalGBufferView;

    agpu_texture_ref specularGBuffer;
    agpu_texture_view_ref specularGBufferView;

    agpu_framebuffer_ref depthOnlyFramebuffer;
    agpu_framebuffer_ref hdrOpaqueFramebuffer;

    agpu_shader_resource_binding_ref guiTextureBinding;
    agpu_shader_resource_binding_ref getValidGuiTextureBinding();

    int screenWidth = -1;
    int screenHeight = -1;
};

/**
 * Scene renderer
 */
class SceneRenderer
{
public:
    static const size_t MaxSceneObjectStateCapacity = 1024;
    static const size_t MaxSceneCameraStateCapacity = 128;

    void renderScene(const agpu_command_list_ref &commandList, const SceneGraph::ScenePtr &scene, const SceneGraph::SceneNodePtr &cameraNode);
    void setupWithScreenSize(int newScreenWidth, int newScreenHeight);

    void useIndexBuffer(const Assets::BinaryBufferAccessorPtr &indices);

    std::shared_ptr<SceneRendererScreen> screen;
    agpu_command_list_ref currentCommandList;
    RenderingScenePtr currentRenderingScene;
    SceneGraph::SceneNodePtr currentCameraNode;

    agpu_shader_resource_binding_ref statesBinding;
    agpu_buffer_ref sceneObjectStatesBuffer;
    agpu_buffer_ref sceneCameraStatesBuffer;

    std::vector<SceneObjectState> sceneObjectStates;
    std::vector<SceneCameraState> sceneCameraStates;

private:
    void addRenderingSceneObjectStateFor(class RenderingSceneObject &sceneObject);
    void gatherRenderingSceneStates();
    void uploadRenderingSceneStates();

    void performDepthOnlyPass();
    void performHDROpaquePass();

};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERER_HPP
