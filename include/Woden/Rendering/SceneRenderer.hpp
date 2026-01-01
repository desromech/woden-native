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

/**
 * Per-screen rendering state.
 */
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

struct ShadowMapAtlasAllocation
{
    Math::Vector2 offset;
    Math::Vector2 shadowMapExtent;
    Math::Vector2 shadowMapAtlasExtent;
};

class ShadowMapAtlasAllocator
{
public:
    void initializeWithExtent(uint32_t atlasWidth, uint32_t atlasHeight);
    void reset();
    bool allocate(ShadowMapAtlasAllocation *outAllocation);
    
    uint32_t atlasWidth;
    uint32_t atlasHeight;

    uint32_t columns;
    uint32_t rows;
    uint32_t capacity;
    uint32_t size;
    Math::Vector2 shadowMapExtent;
};

/**
 * Scene renderer
 */
class SceneRenderer
{
public:
    static const size_t MaxSceneObjectStateCapacity = 1024;
    static const size_t MaxSceneCameraStateCapacity = 32;
    static const size_t MaxSceneLightSourceCapacity = 1024;
    static const size_t MaxMaterialCapacity = 1024;

    static const uint32_t LightGridWidth = 16;
    static const uint32_t LightGridHeight = 9;
    static const uint32_t LightGridDepth = 24;

    static const uint32_t LightGridCellCount = LightGridWidth*LightGridHeight*LightGridDepth;
    static const uint32_t MaxLightClusterCapacity = 100;

    static const uint32_t ShadowMapAtlasSize = 4096;

    void renderScene(const agpu_command_list_ref &commandList, const SceneGraph::ScenePtr &scene, const SceneGraph::SceneNodePtr &cameraNode);
    void setupWithScreenSize(int newScreenWidth, int newScreenHeight);

    void useIndexBuffer(const Assets::BinaryBufferAccessorPtr &indices);

    uint8_t *allocateMaterialStateBuffer(size_t allocationSize, const agpu_shader_resource_binding_ref &materialBinding);

    std::shared_ptr<SceneRendererScreen> screen;
    agpu_command_list_ref currentCommandList;
    RenderingScenePtr currentRenderingScene;
    SceneGraph::SceneNodePtr currentCameraNode;

    agpu_shader_resource_binding_ref statesBinding;
    agpu_buffer_ref sceneObjectStatesBuffer;
    agpu_buffer_ref sceneCameraStatesBuffer;
    agpu_buffer_ref sceneGlobalLightingStateBuffer;

    agpu_buffer_ref sceneWorldLightSourceStatesBuffer;
    agpu_buffer_ref sceneViewLightSourceStatesBuffer;

    agpu_buffer_ref lightClusterBuffer;
    agpu_buffer_ref tileLightIndexListBuffer;
    agpu_buffer_ref lightGridBuffer;

    agpu_texture_ref shadowMapAtlas;
    agpu_framebuffer_ref shadowMapFramebuffer;
    ShadowMapAtlasAllocator shadowMapAtlasAllocator;

    agpu_buffer_ref materialStateBuffer;
    size_t materialStateBufferSize = 0;
    uint8_t *materialStateBufferPointer = nullptr;

    std::vector<SceneObjectState> sceneObjectStates;
    std::vector<SceneCameraState> sceneCameraStates;

    GlobalLightingState globalLightingState;
    std::vector<LightSourceState> sceneLightSourceStates;

private:
    void addRenderingSceneObjectStateFor(class RenderingSceneObject &sceneObject);
    void addRenderingLightSourceObject(class RenderingLightSourceObject &lightSource);
    void gatherRenderingSceneStates();
    void uploadRenderingSceneStates();

    void renderShadowMaps();

    void performDepthOnlyPass();
    void performHDROpaquePass();

};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERER_HPP
