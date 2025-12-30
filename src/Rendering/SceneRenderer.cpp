#include "Woden/Rendering/SceneRenderer.hpp"
#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/Camera.hpp"
#include "Woden/Rendering/Context.hpp"
#include <assert.h>

namespace Woden
{
namespace Rendering
{

inline size_t alignedTo(size_t value, size_t alignment)
{
    return (value + alignment - 1) & size_t(-intptr_t(alignment));
}

agpu_shader_resource_binding_ref SceneRendererScreen::getValidGuiTextureBinding()
{
    if(guiTextureBinding)
        return guiTextureBinding;

    guiTextureBinding = Rendering::RenderingContext::getMainContext()->guiShaderSignature->createShaderResourceBinding(2);
    guiTextureBinding->bindSampledTextureView(0, hdrColorBuffer->getOrCreateFullView());
    return guiTextureBinding;
}

void SceneRenderer::addRenderingSceneObjectStateFor(RenderingSceneObject &sceneObject)
{
    sceneObject.sceneObjectStateIndex = sceneObjectStates.size();

    SceneObjectState objectState = {};
    objectState.transformationMatrix = sceneObject.modelMatrix;
    objectState.inverseTransformationMatrix = sceneObject.inverseModelMatrix;
    sceneObjectStates.push_back(objectState);
}

void SceneRenderer::addRenderingLightSourceObject(class RenderingLightSourceObject &lightSource)
{
    LightSourceState state = {};
    state.positionOrDirection = lightSource.positionOrDirection;
    state.intensity = Math::CompactVector3(lightSource.intensityAndColor);
    state.influenceRadius = lightSource.influenceRadius;
    sceneLightSourceStates.push_back(state);
}

void SceneRenderer::gatherRenderingSceneStates()
{
    sceneObjectStates.clear();
    sceneCameraStates.clear();
    sceneLightSourceStates.clear();

    for(auto &object : currentRenderingScene->backgroundObjects)
        addRenderingSceneObjectStateFor(object);
    for(auto &object : currentRenderingScene->opaqueObjects)
        addRenderingSceneObjectStateFor(object);
    for(auto &object : currentRenderingScene->translucentObject)
        addRenderingSceneObjectStateFor(object);

    for(auto &light : currentRenderingScene->lightSources)
        addRenderingLightSourceObject(light);

    {
        SceneCameraState cameraState = {};
        auto aspect = Math::Scalar(screen->screenWidth) / Math::Scalar(screen->screenHeight);

        if(currentCameraNode && !currentCameraNode->cameras.empty())
        {
            const auto &cameraStateObject = currentCameraNode->cameras.front();
            cameraState.transformationMatrix = currentCameraNode->transform.asMatrix();
            cameraState.inverseTransformationMatrix = currentCameraNode->transform.asInverseMatrix();

            cameraState.projectionMatrix = cameraStateObject->computeProjectionMatrix(aspect);
        }
        else
        {
            cameraState.projectionMatrix = Math::Matrix4x4::ReverseDepthPerspective(60.0, aspect, 0.1, 1000.0);
        }

        if(RenderingContext::getMainContext()->device->hasTopLeftNdcOrigin())
        {
            cameraState.projectionMatrix = Math::Matrix4x4::ProjectionInvertYMatrix() * cameraState.projectionMatrix;
        }
        sceneCameraStates.push_back(cameraState);
    }
}

void SceneRenderer::uploadRenderingSceneStates()
{
    assert(sceneObjectStates.size() <= MaxSceneObjectStateCapacity);
    assert(sceneCameraStates.size() <= MaxSceneCameraStateCapacity);

    if(!statesBinding)
    {
        auto context = RenderingContext::getMainContext();
        statesBinding = context->sceneShaderSignature->createShaderResourceBinding(1);

        {
            agpu_buffer_description desc = {};
            desc.size = sizeof(SceneObjectState)*MaxSceneObjectStateCapacity;
            desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
            desc.usage_modes = desc.main_usage_mode = agpu_buffer_usage_mask(AGPU_STORAGE_BUFFER);
            desc.mapping_flags = AGPU_MAP_DYNAMIC_STORAGE_BIT;
            desc.stride = 0;

            sceneObjectStatesBuffer = context->device-> createBuffer(&desc, nullptr);
            statesBinding->bindStorageBuffer(0, sceneObjectStatesBuffer);
        }

        {
            agpu_buffer_description desc = {};
            desc.size = sizeof(SceneCameraState)*MaxSceneCameraStateCapacity;
            desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
            desc.usage_modes = desc.main_usage_mode = agpu_buffer_usage_mask(AGPU_STORAGE_BUFFER);
            desc.mapping_flags = AGPU_MAP_DYNAMIC_STORAGE_BIT;
            desc.stride = 0;

            sceneCameraStatesBuffer = context->device-> createBuffer(&desc, nullptr);
            statesBinding->bindStorageBuffer(1, sceneCameraStatesBuffer);
        }

        {
            agpu_buffer_description desc = {};
            desc.size = alignedTo(sizeof(GlobalLightingState), 256);
            desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
            desc.usage_modes = desc.main_usage_mode = agpu_buffer_usage_mask(AGPU_UNIFORM_BUFFER);
            desc.mapping_flags = AGPU_MAP_DYNAMIC_STORAGE_BIT;
            desc.stride = 0;

            sceneGlobalLightingStateBuffer = context->device-> createBuffer(&desc, nullptr);
            statesBinding->bindUniformBuffer(2, sceneGlobalLightingStateBuffer);
        }

        {
            agpu_buffer_description desc = {};
            desc.size = sizeof(LightSourceState)*MaxSceneLightSourceCapacity;
            desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
            desc.usage_modes = desc.main_usage_mode = agpu_buffer_usage_mask(AGPU_STORAGE_BUFFER);
            desc.mapping_flags = AGPU_MAP_DYNAMIC_STORAGE_BIT;
            desc.stride = 0;

            sceneLightSourceStatesBuffer = context->device-> createBuffer(&desc, nullptr);
            statesBinding->bindStorageBuffer(3, sceneLightSourceStatesBuffer);
        }
          
    }
    
    sceneObjectStatesBuffer->uploadBufferData(0, sizeof(SceneObjectState)*sceneObjectStates.size(), sceneObjectStates.data());
    sceneCameraStatesBuffer->uploadBufferData(0, sizeof(SceneCameraState)*sceneCameraStates.size(), sceneCameraStates.data());

    globalLightingState.numberOfLights = sceneLightSourceStates.size();
    sceneGlobalLightingStateBuffer->uploadBufferData(0, sizeof(GlobalLightingState), &globalLightingState);
    sceneLightSourceStatesBuffer->uploadBufferData(0, sizeof(LightSourceState)*sceneLightSourceStates.size(), sceneLightSourceStates.data());
}

void SceneRenderer::renderScene(const agpu_command_list_ref &commandList, const SceneGraph::ScenePtr &scene, const SceneGraph::SceneNodePtr &cameraNode)
{
    auto context = RenderingContext::getMainContext();
    currentCommandList = commandList;

    currentRenderingScene = std::make_shared<RenderingScene> ();
    currentCameraNode = cameraNode;
    currentRenderingScene->currentInverseViewMatrix = currentCameraNode->transform.asMatrix();
    currentRenderingScene->currentViewMatrix = currentCameraNode->transform.asInverseMatrix();
    scene->addIntoRenderingScene(currentRenderingScene);

    // Gather and rendering scene states.
    gatherRenderingSceneStates();
    uploadRenderingSceneStates();

    // Depth-Stencil render pass.
    commandList->beginRenderPass(context->depthStencilRenderPass, screen->depthOnlyFramebuffer, false);
    commandList->setViewport(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setScissor(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setShaderSignature(context->sceneShaderSignature);
    commandList->useShaderResources(statesBinding);

    ScenePushConstants initialPushConstants = {};
    commandList->pushConstants(0, sizeof(initialPushConstants), &initialPushConstants);

    performDepthOnlyPass();

    commandList->endRenderPass();

    // HDR opaque render pass.
    commandList->beginRenderPass(context->hdrOpaqueRenderPass, screen->hdrOpaqueFramebuffer, false);
    commandList->setViewport(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setScissor(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setShaderSignature(context->sceneShaderSignature);
    commandList->useShaderResources(statesBinding);
    commandList->pushConstants(0, sizeof(initialPushConstants), &initialPushConstants);

    performHDROpaquePass();

    commandList->endRenderPass();
    currentCommandList.reset();
}


void SceneRenderer::performDepthOnlyPass()
{
    for(auto &object : currentRenderingScene->opaqueObjects)
    {
        currentCommandList->pushConstants(0, 4, &object.sceneObjectStateIndex);
        object.renderable->renderDepthOnlyWith(this);
    }
}

void SceneRenderer::performHDROpaquePass()
{
    for(auto &object : currentRenderingScene->backgroundObjects)
    {
        currentCommandList->pushConstants(0, 4, &object.sceneObjectStateIndex);
        object.renderable->renderOpaqueWith(this);
    }

    for(auto &object : currentRenderingScene->opaqueObjects)
    {
        currentCommandList->pushConstants(0, 4, &object.sceneObjectStateIndex);
        object.renderable->renderOpaqueWith(this);
    }
}

void SceneRenderer::useIndexBuffer(const Assets::BinaryBufferAccessorPtr &indices)
{
    auto bufferHandle = indices->bufferView->buffer->getOrCreateBufferWithContents();
    currentCommandList->useIndexBufferAt(
        bufferHandle,
        indices->bufferView->byteOffset + indices->byteOffset,
        indices->bufferView->byteStride
    );
    
}

void SceneRenderer::setupWithScreenSize(int newScreenWidth, int newScreenHeight)
{
    if(screen && screen->screenHeight == newScreenHeight && screen->screenWidth == newScreenWidth)
        return;

    // Ensure to finish the execution, so that we can release existing resources.
    auto device = RenderingContext::getMainContext()->device;
    device->finishExecution();

    // Create the new screen;
    screen = std::make_shared<SceneRendererScreen> ();
    screen->screenWidth = newScreenWidth;
    screen->screenHeight = newScreenHeight;

    // Depth stencil buffers
    {
        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = newScreenWidth;
        desc.height = newScreenHeight;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = RenderingContext::DepthStencilBufferFormat;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_DEPTH_ATTACHMENT | AGPU_TEXTURE_USAGE_STENCIL_ATTACHMENT | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.sample_count = 1;
        desc.clear_value.depth_stencil.depth = 0;
        screen->depthStencilBuffer = device->createTexture(&desc);
        if(!screen->depthStencilBuffer)
        {
            fprintf(stderr, "Failed to create depth stencil buffer\n");
            abort();
        }
    }

    {
        agpu_texture_view_description viewDesc = {};
        screen->depthStencilBuffer->getFullViewDescription(&viewDesc);
        viewDesc.format = RenderingContext::DepthStencilBufferViewFormat;

        screen->depthStencilAttachmentView = screen->depthStencilBuffer->createView(&viewDesc);
        if(!screen->depthStencilAttachmentView)
        {
            fprintf(stderr, "Failed to create depth stencil buffer view\n");
            abort();
        }
    }
    
    // HDR Color Buffer
    {
        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = newScreenWidth;
        desc.height = newScreenHeight;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = RenderingContext::HDRColorBufferFormat;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_COLOR_ATTACHMENT | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.sample_count = 1;
        desc.clear_value.depth_stencil.depth = 0;
        screen->hdrColorBuffer = device->createTexture(&desc);
        if(!screen->hdrColorBuffer)
        {
            fprintf(stderr, "Failed to create HDR color buffer\n");
            abort();
        }

        agpu_texture_view_description attachmentViewDesc = {};
        screen->hdrColorBuffer->getFullViewDescription(&attachmentViewDesc);
        attachmentViewDesc.usage_mode = AGPU_TEXTURE_USAGE_COLOR_ATTACHMENT;

        screen->hdrColorBufferView = screen->hdrColorBuffer->createView(&attachmentViewDesc);
    }   

    // Normal G-Buffer
    {
        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = newScreenWidth;
        desc.height = newScreenHeight;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = RenderingContext::NormalGBufferFormat;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_COLOR_ATTACHMENT | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.sample_count = 1;
        screen->normalGBuffer = device->createTexture(&desc);
        if(!screen->normalGBuffer)
        {
            fprintf(stderr, "Failed to create the normal G-buffer\n");
            abort();
        }

        agpu_texture_view_description attachmentViewDesc = {};
        screen->normalGBuffer->getFullViewDescription(&attachmentViewDesc);
        attachmentViewDesc.usage_mode = AGPU_TEXTURE_USAGE_COLOR_ATTACHMENT;

        screen->normalGBufferView = screen->normalGBuffer->createView(&attachmentViewDesc);
    }

    // Specular G-Buffer
    {
        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = newScreenWidth;
        desc.height = newScreenHeight;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = RenderingContext::SpecularGBufferFormat;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_COLOR_ATTACHMENT | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.sample_count = 1;
        screen->specularGBuffer = device->createTexture(&desc);
        if(!screen->specularGBuffer)
        {
            fprintf(stderr, "Failed to create the normal G-buffer\n");
            abort();
        }

        agpu_texture_view_description attachmentViewDesc = {};
        screen->specularGBuffer->getFullViewDescription(&attachmentViewDesc);
        attachmentViewDesc.usage_mode = AGPU_TEXTURE_USAGE_COLOR_ATTACHMENT;

        screen->specularGBufferView = screen->specularGBuffer->createView(&attachmentViewDesc);
    }
    

    // Depth-Stencil only framebuffer
    {
        screen->depthOnlyFramebuffer = device->createFrameBuffer(newScreenWidth, newScreenHeight, 0, nullptr, screen->depthStencilAttachmentView);
        if(!screen->depthOnlyFramebuffer)
        {
            fprintf(stderr, "Failed to create depth only framebuffer\n");
            abort();
        }
    }

    // HDR-depth framebuffer
    {
        agpu_texture_view_ref colorAttachments[3] = {
            screen->hdrColorBufferView,
            screen->normalGBufferView,
            screen->specularGBufferView
        };

        screen->hdrOpaqueFramebuffer = device->createFrameBuffer(newScreenWidth, newScreenHeight, 3, colorAttachments, screen->depthStencilAttachmentView);
        if(!screen->hdrOpaqueFramebuffer)
        {
            fprintf(stderr, "Failed to create hdr-opaque framebuffer\n");
            abort();
        }
    }

}

} // End of namespace Rendering
} // End of namespace Woden