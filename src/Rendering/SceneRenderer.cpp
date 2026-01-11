#include "Woden/Rendering/SceneRenderer.hpp"
#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/Camera.hpp"
#include "Woden/Rendering/Context.hpp"
#include <assert.h>

namespace Woden
{
namespace Rendering
{


void ShadowMapAtlasAllocator::initializeWithExtent(uint32_t atlasWidth, uint32_t atlasHeight)
{
    this->atlasWidth = atlasWidth;
    this->atlasHeight = atlasHeight;

    columns = 4;
    rows = 4;

    shadowMapExtent.x = Math::Scalar(atlasWidth) / Math::Scalar(columns);
    shadowMapExtent.y = Math::Scalar(atlasHeight) / Math::Scalar(rows);

    capacity = columns*rows;
    size = 0;
}

void ShadowMapAtlasAllocator::reset()
{
    size = 0;
}

bool ShadowMapAtlasAllocator::allocate(ShadowMapAtlasAllocation *outAllocation)
{
    if(size >= capacity)
        return false;

    ShadowMapAtlasAllocation empty = {};
    *outAllocation = empty;

    int row = size / columns;
    int column = size % columns;
    
    outAllocation->offset.x = column*shadowMapExtent.x;
    outAllocation->offset.y = row*shadowMapExtent.y;
    
    outAllocation->shadowMapExtent = shadowMapExtent;

    outAllocation->shadowMapAtlasExtent.x = Math::Scalar(atlasWidth);
    outAllocation->shadowMapAtlasExtent.y = Math::Scalar(atlasHeight);

    ++size;

    return true;
}

static Math::Scalar computeLightGridDepthSliceScale(Math::Scalar lightGridDepth, Math::Scalar nearDistance, Math::Scalar farDistance)
{
    return lightGridDepth / log(farDistance / nearDistance);
}

static Math::Scalar computeLightGridDepthSliceOffset(Math::Scalar lightGridDepth, Math::Scalar nearDistance, Math::Scalar farDistance)
{
    return -lightGridDepth * log(nearDistance) / log(farDistance / nearDistance);
}

static Math::Vector2 computeLightGridDepthSliceScaleOffset(Math::Scalar lightGridDepth, Math::Scalar nearDistance, Math::Scalar farDistance)
{
    Math::Scalar scale = computeLightGridDepthSliceScale(lightGridDepth, nearDistance, farDistance);
    Math::Scalar offset = computeLightGridDepthSliceOffset(lightGridDepth, nearDistance, farDistance);
    
    // Formula from: http://www.aortiz.me/2018/12/21/CG.html#building-a-cluster-grid [January 2025]
    return Math::Vector2(scale, offset);
}

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
    sceneObject.sceneObjectStateIndex = uint32_t(sceneObjectStates.size());

    SceneObjectState objectState = {};
    objectState.transformationMatrix = sceneObject.modelMatrix;
    objectState.inverseTransformationMatrix = sceneObject.inverseModelMatrix;
    sceneObjectStates.push_back(objectState);
}

void SceneRenderer::addRenderingLightSourceObject(class RenderingLightSourceObject &lightSource)
{
    auto device = RenderingContext::getMainContext()->device;
    bool flipTextureVertically = device->hasTopLeftNdcOrigin() == device->hasBottomLeftTextureCoordinates();

    LightSourceState state = {};
    state.positionOrDirection = lightSource.positionOrDirection;
    
    state.intensity = Math::CompactVector3(lightSource.intensityAndColor);
    state.influenceRadius = lightSource.influenceRadius;

    state.spotDirection = lightSource.spotDirection;
    state.innerSpotCosCutoff = lightSource.innerSpotCosCutoff;

    state.outerSpotCosCutoff = lightSource.outerSpotCosCutoff;
    state.castShadows = lightSource.castShadows ? 1 : 0;

    if (lightSource.shadowMapPartCount == 0)
    {
        state.castShadows = false;
    }
    else if(state.castShadows)
    {
        bool hasAllocatedAllParts = true;
        state.shadowMapNormalBiasFactor = lightSource.shadowMapNormalBiasFactor;
        state.shadowMapCascadeDistanceWorldTransform = lightSource.shadowMapCascadeDistanceWorldTransform;
        state.shadowMapCascadeOffsets = lightSource.shadowMapCascadeOffsets;

        for(uint32_t partIndex = 0; partIndex < lightSource.shadowMapPartCount; ++partIndex)
        {
            ShadowMapAtlasAllocation allocation = {};
            if(!shadowMapAtlasAllocator.allocate(&allocation))
            {
                hasAllocatedAllParts = false;
                break;
            }

            Math::Vector2 viewportScale = Math::Vector2(0.5, 0.5);
            if(flipTextureVertically)
                viewportScale.y = -viewportScale.y;
            Math::Vector2 viewportOffset = Math::Vector2(0.5, 0.5); 

            Math::Vector2 atlasExtent =  allocation.shadowMapAtlasExtent;
            Math::Vector2 viewportExtent = allocation.shadowMapExtent;
            Math::Vector2 viewportExtentScale = Math::Vector2(viewportExtent.x / atlasExtent.x, viewportExtent.y / atlasExtent.y);

            Math::Vector2 shadowMapViewportScale = Math::Vector2(viewportScale.x *viewportExtentScale.x, viewportScale.y *viewportExtentScale.y);
            state.shadowMapViewportScale = shadowMapViewportScale;
            state.shadowMapViewportOffsets[partIndex] = viewportOffset*viewportExtentScale + allocation.offset/allocation.shadowMapAtlasExtent;

            state.modelMatrix[partIndex] = lightSource.modelMatrix[partIndex];
            state.inverseModelMatrix[partIndex] = lightSource.inverseModelMatrix[partIndex];

            state.projectionMatrix[partIndex] = lightSource.projectionMatrix[partIndex];
            state.inverseProjectionMatrix[partIndex] = lightSource.inverseProjectionMatrix[partIndex];

            lightSource.atlasAllocations[partIndex] = allocation;
        }

        // Disable shadows from a light source that does not have it allocated.
        if(!hasAllocatedAllParts)
        {
            state.castShadows = false;
            lightSource.castShadows = false;
        }
    }

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
        auto device = RenderingContext::getMainContext()->device;
        uint32_t flipVertically = device->hasTopLeftNdcOrigin();

        SceneCameraState cameraState = {};
        auto aspect = Math::Scalar(screen->screenWidth) / Math::Scalar(screen->screenHeight);
        cameraState.framebufferExtent = Math::Vector2(Math::Scalar(screen->screenWidth), Math::Scalar(screen->screenHeight));
        cameraState.framebufferReciprocalExtent = cameraState.framebufferExtent.reciprocal();
        cameraState.flipVertically = flipVertically;

        cameraState.hasTopLeftNDCOrigin = device->hasTopLeftNdcOrigin();
    	cameraState.hasBottomLeftTextureCoordinates = device->hasBottomLeftTextureCoordinates();

        cameraState.lightGridWidth = LightGridWidth;
        cameraState.lightGridHeight = LightGridHeight;
        cameraState.lightGridDepth = LightGridDepth;

        if(currentCameraNode && !currentCameraNode->cameras.empty())
        {
            const auto &camera = currentCameraNode->cameras.front();

            const auto &cameraStateObject = currentCameraNode->cameras.front();
            cameraState.nearDistance = camera->nearDistance;
            cameraState.farDistance = camera->farDistance;

            
            cameraState.transformationMatrix = currentCameraNode->computeGlobalTransformMatrix();
            cameraState.inverseTransformationMatrix = cameraState.transformationMatrix.inverse();

            cameraState.projectionMatrix = cameraStateObject->computeProjectionMatrix(aspect);
        }
        else
        {
            cameraState.projectionMatrix = Math::Matrix4x4::ReverseDepthPerspective(60.0, aspect, 0.1f, 1000.0);
        }

        if(flipVertically)
            cameraState.projectionMatrix = Math::Matrix4x4::ProjectionInvertYMatrix() * cameraState.projectionMatrix;
        cameraState.inverseProjectionMatrix = cameraState.projectionMatrix.inverse();

        cameraState.lightGridDepthSliceScaleOffset = computeLightGridDepthSliceScaleOffset(LightGridDepth, cameraState.nearDistance, cameraState.farDistance);

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
            desc.size = agpu_uint(alignedTo(sizeof(GlobalLightingState), 256));
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

            sceneWorldLightSourceStatesBuffer = context->device->createBuffer(&desc, nullptr);
            statesBinding->bindStorageBuffer(3, sceneWorldLightSourceStatesBuffer);
        }

        {
            agpu_buffer_description desc = {};
            desc.size = sizeof(LightSourceState)*MaxSceneLightSourceCapacity;
            desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
            desc.usage_modes = desc.main_usage_mode = AGPU_STORAGE_BUFFER;
            desc.stride = 0;

            sceneViewLightSourceStatesBuffer = context->device->createBuffer(&desc, nullptr);
            statesBinding->bindStorageBuffer(4, sceneViewLightSourceStatesBuffer);
        }

        {
            agpu_buffer_description desc = {};
            desc.size = sizeof(LightCluster)*LightGridCellCount;
            desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
            desc.usage_modes = desc.main_usage_mode = AGPU_STORAGE_BUFFER;
            desc.stride = 0;

            lightClusterBuffer = context->device->createBuffer(&desc, nullptr);
            statesBinding->bindStorageBuffer(5, lightClusterBuffer);
        }

        {
            agpu_buffer_description desc = {};
            desc.size =  4 * MaxLightClusterCapacity * LightGridCellCount;
            desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
            desc.usage_modes = desc.main_usage_mode = AGPU_STORAGE_BUFFER;
            desc.stride = 0;

            tileLightIndexListBuffer = context->device->createBuffer(&desc, nullptr);
            statesBinding->bindStorageBuffer(6, tileLightIndexListBuffer);
        }

        {
            agpu_buffer_description desc = {};
            desc.size = 8 + 8 * MaxLightClusterCapacity * LightGridCellCount;
            desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
            desc.usage_modes = desc.main_usage_mode = AGPU_STORAGE_BUFFER;
            desc.stride = 0;

            lightGridBuffer = context->device->createBuffer(&desc, nullptr);
            statesBinding->bindStorageBuffer(7, lightGridBuffer);
        }

        // Shadow map atlas
        {   
            agpu_texture_description desc = {};
            desc.type = AGPU_TEXTURE_2D;
            desc.width = ShadowMapAtlasSize;
            desc.height = ShadowMapAtlasSize;
            desc.depth = 1;
            desc.layers = 1;
            desc.miplevels = 1;
            desc.format = RenderingContext::ShadowMapAtlasFormat;
            desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_DEPTH_ATTACHMENT | AGPU_TEXTURE_USAGE_SAMPLED);
            desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
            desc.sample_count = 1;
            desc.clear_value.depth_stencil.depth = 0;
            shadowMapAtlas = context->device->createTexture(&desc);
            if(!shadowMapAtlas)
            {
                fprintf(stderr, "Failed to create the shadow map atlas texture\n");
                abort();
            }

            {
                agpu_texture_view_description viewDesc = {};
                shadowMapAtlas->getFullViewDescription(&viewDesc);
                viewDesc.format = RenderingContext::ShadowMapAtlasSampledFormat;
                shadowMapAtlasSampledView = shadowMapAtlas->createView(&viewDesc);
                statesBinding->bindSampledTextureView(8, shadowMapAtlasSampledView);
            }

            {
                agpu_texture_view_description viewDesc = {};
                shadowMapAtlas->getFullViewDescription(&viewDesc);
                viewDesc.format = RenderingContext::ShadowMapAtlasViewFormat;
                viewDesc.usage_mode = AGPU_TEXTURE_USAGE_DEPTH_ATTACHMENT;
                auto depthView = shadowMapAtlas->createView(&viewDesc);
                
                shadowMapFramebuffer = context->device->createFrameBuffer(ShadowMapAtlasSize, ShadowMapAtlasSize, 0, nullptr, depthView);
            }
        }
    }
    
    sceneObjectStatesBuffer->uploadBufferData(0, sizeof(SceneObjectState)*sceneObjectStates.size(), sceneObjectStates.data());
    sceneCameraStatesBuffer->uploadBufferData(0, sizeof(SceneCameraState)*sceneCameraStates.size(), sceneCameraStates.data());

    globalLightingState.numberOfLights = sceneLightSourceStates.size();
    sceneGlobalLightingStateBuffer->uploadBufferData(0, sizeof(GlobalLightingState), &globalLightingState);
    sceneWorldLightSourceStatesBuffer->uploadBufferData(0, sizeof(LightSourceState)*sceneLightSourceStates.size(), sceneLightSourceStates.data());
}

void SceneRenderer::renderScene(const agpu_command_list_ref &commandList, const SceneGraph::ScenePtr &scene, const SceneGraph::SceneNodePtr &cameraNode)
{
    auto context = RenderingContext::getMainContext();
    currentCommandList = commandList;

    currentRenderingScene = std::make_shared<RenderingScene> ();
    currentCameraNode = cameraNode;

    currentRenderingScene->currentInverseViewMatrix = currentCameraNode->computeGlobalTransformMatrix();
    currentRenderingScene->currentViewMatrix = currentRenderingScene->currentInverseViewMatrix.inverse();

    if(!currentCameraNode->cameras.empty())
    {
        auto &camera = currentCameraNode->cameras.front();
        auto aspect = Math::Scalar(screen->screenWidth) / Math::Scalar(screen->screenHeight);
        currentRenderingScene->currentViewFrustum = camera->computeViewFrustum(aspect);
        currentRenderingScene->currentWorldFrustum = currentRenderingScene->currentViewFrustum.transformedWithMatrix(currentRenderingScene->currentInverseViewMatrix);
        currentRenderingScene->currentCamera = camera;
    }

    if(scene)
        scene->addIntoRenderingScene(currentRenderingScene);

    // Reset the shadow map atlas4
    shadowMapAtlasAllocator.initializeWithExtent(ShadowMapAtlasSize, ShadowMapAtlasSize);

    // Gather and rendering scene states.
    gatherRenderingSceneStates();
    uploadRenderingSceneStates();
    renderShadowMaps();

    ScenePushConstants initialPushConstants = {};

    // Compute the lighting grid.
    {
        // Transform lights from world space into view space.
        commandList->setShaderSignature(context->sceneShaderSignature);
        commandList->usePipelineState(context->transformLightsToViewPipeline);
        commandList->useComputeShaderResources(statesBinding);
        commandList->pushConstants(0, sizeof(initialPushConstants), &initialPushConstants);
        commandList->dispatchCompute((sceneLightSourceStates.size() + 127)/128, 1, 1);
        commandList->memoryBarrier(AGPU_PIPELINE_STAGE_COMPUTE_SHADER, agpu_pipeline_stage_flags(AGPU_PIPELINE_STAGE_COMPUTE_SHADER | AGPU_PIPELINE_STAGE_FRAGMENT_SHADER), AGPU_ACCESS_SHADER_WRITE, AGPU_ACCESS_SHADER_READ);

        // Light grid computation.
        uint32_t workgroupCountX = (LightGridWidth + 3) / 4;
        uint32_t workgroupCountY = (LightGridHeight + 3) / 4;
        uint32_t workgroupCountZ = (LightGridDepth + 3) / 4;
        
        uint32_t workgroupCount = (LightGridCellCount + 63) / 64;

        commandList->usePipelineState(context->lightGridComputationPipeline);
        commandList->dispatchCompute(workgroupCountX, workgroupCountY, workgroupCountZ);
        commandList->memoryBarrier(AGPU_PIPELINE_STAGE_COMPUTE_SHADER, AGPU_PIPELINE_STAGE_COMPUTE_SHADER, AGPU_ACCESS_SHADER_WRITE, AGPU_ACCESS_SHADER_READ);

        commandList->usePipelineState(context->lightClusterBeginComputationPipeline);
        commandList->dispatchCompute(1, 1, 1);
        commandList->memoryBarrier(AGPU_PIPELINE_STAGE_COMPUTE_SHADER, AGPU_PIPELINE_STAGE_COMPUTE_SHADER, AGPU_ACCESS_SHADER_WRITE, AGPU_ACCESS_SHADER_READ);

        commandList->usePipelineState(context->lightClusterListComputationPipeline);
        commandList->dispatchCompute(workgroupCount, 1, 1);
        commandList->memoryBarrier(AGPU_PIPELINE_STAGE_COMPUTE_SHADER, agpu_pipeline_stage_flags(AGPU_PIPELINE_STAGE_COMPUTE_SHADER | AGPU_PIPELINE_STAGE_FRAGMENT_SHADER), AGPU_ACCESS_SHADER_WRITE, AGPU_ACCESS_SHADER_READ);
    }

    // Depth-Stencil render pass.
    commandList->beginRenderPass(context->depthStencilRenderPass, screen->depthOnlyFramebuffer, false);
    commandList->setViewport(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setScissor(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setShaderSignature(context->sceneShaderSignature);
    commandList->useShaderResources(context->sceneSamplerBindings);
    commandList->useShaderResources(statesBinding);
    commandList->pushConstants(0, sizeof(initialPushConstants), &initialPushConstants);

    performDepthOnlyPass();

    commandList->endRenderPass();

    // HDR opaque render pass.
    commandList->beginRenderPass(context->hdrOpaqueRenderPass, screen->hdrOpaqueFramebuffer, false);
    commandList->setViewport(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setScissor(0, 0, screen->screenWidth, screen->screenHeight);
    commandList->setShaderSignature(context->sceneShaderSignature);
    commandList->useShaderResources(context->sceneSamplerBindings);
    commandList->useShaderResources(statesBinding);
    commandList->pushConstants(0, sizeof(initialPushConstants), &initialPushConstants);

    performHDROpaquePass();

    commandList->endRenderPass();
    currentCommandList.reset();
}

void SceneRenderer::renderShadowMaps()
{
    auto context = RenderingContext::getMainContext();
    currentCommandList->beginRenderPass(context->shadowMapRenderPass, shadowMapFramebuffer, false);

    assert(currentRenderingScene->lightSources.size() == sceneLightSourceStates.size());

    for(size_t lightIndex = 0; lightIndex < sceneLightSourceStates.size(); ++lightIndex)
    {
        auto &lightState = sceneLightSourceStates[lightIndex];
        if(!lightState.castShadows)
            continue;

        auto &renderingLight = currentRenderingScene->lightSources[lightIndex];
     
        for(uint32_t shadowPartIndex = 0; shadowPartIndex < renderingLight.shadowMapPartCount; ++shadowPartIndex)
        {
            auto &allocation = renderingLight.atlasAllocations[shadowPartIndex];

            currentCommandList->setViewport(allocation.offset.x, allocation.offset.y, allocation.shadowMapExtent.x, allocation.shadowMapExtent.y);
            currentCommandList->setScissor(allocation.offset.x, allocation.offset.y, allocation.shadowMapExtent.x, allocation.shadowMapExtent.y);
            currentCommandList->setShaderSignature(context->sceneShaderSignature);
            currentCommandList->useShaderResources(context->sceneSamplerBindings);
            currentCommandList->useShaderResources(statesBinding);

            ScenePushConstants shadowPushConstants = {};
            shadowPushConstants.lightStateIndex = lightIndex;
            shadowPushConstants.lightStateComponentIndex = shadowPartIndex;
            currentCommandList->pushConstants(0, sizeof(shadowPushConstants), &shadowPushConstants);

            // Clear the shadow map.
            currentCommandList->usePipelineState(context->clearDepthPipelineState);
            currentCommandList->drawArrays(4, 1, 0, 0);

            performShadowPass();
        }
    }

    currentCommandList->endRenderPass();
}
void SceneRenderer::performShadowPass()
{
    for(auto &object : currentRenderingScene->opaqueObjects)
    {
        currentCommandList->pushConstants(0, 4, &object.sceneObjectStateIndex);
        object.renderable->renderShadowWith(this);
    }
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

uint8_t *SceneRenderer::allocateMaterialStateBuffer(size_t allocationSize, const agpu_shader_resource_binding_ref &materialBinding)
{
    size_t alignedAllocationSize = alignedTo(allocationSize, 256);

    if(!materialStateBuffer)
    {
        agpu_buffer_description desc = {};
        desc.size = 256*MaxMaterialCapacity;
        desc.heap_type = AGPU_MEMORY_HEAP_TYPE_HOST_TO_DEVICE;
        desc.usage_modes = desc.main_usage_mode = agpu_buffer_usage_mask(AGPU_UNIFORM_BUFFER);
        desc.mapping_flags = AGPU_MAP_WRITE_BIT | AGPU_MAP_PERSISTENT_BIT | AGPU_MAP_COHERENT_BIT;
        desc.stride = 0;

        materialStateBuffer = Rendering::RenderingContext::getMainContext()->device->createBuffer(&desc, nullptr);
        materialStateBufferPointer = (uint8_t*)materialStateBuffer->mapBuffer(AGPU_WRITE_ONLY);
    }

    auto bufferOffset = materialStateBufferSize;
    auto resultPointer = materialStateBufferPointer + materialStateBufferSize;
    materialStateBufferSize += alignedAllocationSize;

    materialBinding->bindUniformBufferRange(0, materialStateBuffer, bufferOffset, alignedAllocationSize);

    return resultPointer;
}

} // End of namespace Rendering
} // End of namespace Woden