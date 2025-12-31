#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/VertexBinding.hpp"
#include "Woden/Utility/ReadWholeFile.hpp"
#include <vector>
#include <stdio.h>

namespace Woden
{
namespace Rendering
{

RenderingContextPtr RenderingContext::mainContext;

RenderingContextPtr RenderingContext::getMainContext()
{
    return mainContext;
}

RenderingContextPtr RenderingContext::getOrCreateMainContext(int argc, const char *argv[])
{
    if(mainContext)
        return mainContext;

    auto newContext = std::make_shared<RenderingContext> ();
    if(newContext->initialize(argc, argv))
        mainContext = newContext;

    return mainContext;
}

void RenderingContext::releaseMainContext()
{
    mainContext.reset();
}


bool RenderingContext::initialize(int argc, const char *argv[])
{
    bool debugLayerEnabled = false;
#ifdef _DEBUG
    debugLayerEnabled= true;
#endif
    agpu_uint platformIndex = 0;
    agpu_uint gpuIndex = 0;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-no-vsync")
        {
            vsyncDisabled = true;
        }
        else if (arg == "-platform")
        {
            platformIndex = agpu_uint(atoi(argv[++i]));
        }
        else if (arg == "-gpu")
        {
            gpuIndex = agpu_uint(atoi(argv[++i]));
        }
        else if (arg == "-debug")
        {
            debugLayerEnabled = true;
        }
    }

        // Get the platform.
    agpu_uint numPlatforms;
    agpuGetPlatforms(0, nullptr, &numPlatforms);
    if (numPlatforms == 0)
    {
        fprintf(stderr, "No agpu platforms are available.\n");
        return false;
    }
    else if (platformIndex >= numPlatforms)
    {
        fprintf(stderr, "Selected platform index is not available.\n");
        return false;
    }

    std::vector<agpu_platform*> platforms;
    platforms.resize(numPlatforms);
    agpuGetPlatforms(numPlatforms, &platforms[0], nullptr);
    auto platform = platforms[platformIndex];

    printf("Chosen platform: %s\n", agpuGetPlatformName(platform));

    agpu_device_open_info deviceOpenInfo = {};
    deviceOpenInfo.gpu_index = gpuIndex;
    deviceOpenInfo.debug_layer = debugLayerEnabled;

    device = platform->openDevice(&deviceOpenInfo);
    if(!device)
        return false;

    printf("Chosen device: %s\n", device->getName());

    defaultCommandQueue = device->getDefaultCommandQueue();

    // Create the window render pass
    {
        // Color attachment
        agpu_renderpass_color_attachment_description colorAttachment = {};
        colorAttachment.format = WindowColorBufferFormat;
        colorAttachment.begin_action = AGPU_ATTACHMENT_CLEAR;
        colorAttachment.end_action = AGPU_ATTACHMENT_KEEP;
        colorAttachment.sample_count = 1;


        agpu_renderpass_description description = {};
        description.color_attachment_count = 1;
        description.color_attachments = &colorAttachment;

        windowRenderPass = device->createRenderPass(&description);
        if(!windowRenderPass)
            return false;
    }

    if(!createGuiPipelineStates())
        return false;

    if(!loadFonts())
        return false;
    
    if(!createScenePipelineStates())
        return false;

    return true;
}

agpu_shader_ref RenderingContext::compileShader(const std::string &sharedCommon, const std::string &shaderFileName, agpu_shader_type type)
{
    return compileShader(sharedCommon, std::string(), shaderFileName, type);
}

agpu_shader_ref RenderingContext::compileShader(const std::string &sharedCommon, const std::string &sharedCommon2, const std::string &shaderFileName, agpu_shader_type type)
{
    std::string shaderSource;
    if(!sharedCommon.empty())
        shaderSource += Woden::Utility::readWholeTextFile(sharedCommon);
    if(!sharedCommon2.empty())
        shaderSource += Woden::Utility::readWholeTextFile(sharedCommon2);
    if(!shaderFileName.empty())
        shaderSource += Woden::Utility::readWholeTextFile(shaderFileName);
   if(shaderSource.empty())
        return nullptr;

    // Create the shader compiler.
    agpu_offline_shader_compiler_ref shaderCompiler = device->createOfflineShaderCompiler();
    shaderCompiler->setShaderSource(AGPU_SHADER_LANGUAGE_VGLSL, type, shaderSource.c_str(), (agpu_string_length)shaderSource.size());
    try
    {
        shaderCompiler->compileShader(AGPU_SHADER_LANGUAGE_DEVICE_SHADER, nullptr);
    }
    catch(agpu_exception &e)
    {
        auto logLength = shaderCompiler->getCompilationLogLength();
        std::unique_ptr<char[]> logBuffer(new char[logLength+1]);
        shaderCompiler->getCompilationLog(logLength+1, logBuffer.get());
        fprintf(stderr, "Compilation error of '%s':%s\n", shaderFileName.c_str(), logBuffer.get());
        return nullptr;
    }

    // Create the shader and compile it.
    return shaderCompiler->getResultAsShader();
}

bool RenderingContext::createGuiPipelineStates()
{
    // Create the GUI shader signature.
    {
        auto shaderSignatureBuilder = device->createShaderSignatureBuilder();

        shaderSignatureBuilder->beginBindingBank(1);
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLER, 1); // Linear
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLER, 1); // Nearest

        shaderSignatureBuilder->beginBindingBank(128);
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_STORAGE_BUFFER, 1); // GUI Elements

        shaderSignatureBuilder->beginBindingBank(1024);
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // GUI Textures

        shaderSignatureBuilder->addBindingConstant(); // hasTopLeftNDCOrigin
        shaderSignatureBuilder->addBindingConstant(); // padding
        shaderSignatureBuilder->addBindingConstant(); // framebufferReciprocalExtent x
        shaderSignatureBuilder->addBindingConstant(); // framebufferReciprocalExtent y

        guiShaderSignature = shaderSignatureBuilder->build();
        if (!guiShaderSignature)
            return false;
    }

    // GUI samplers
    {
        guiSamplerBindings = guiShaderSignature->createShaderResourceBinding(0);

        {
            agpu_sampler_description desc = {};
            desc.address_u = AGPU_TEXTURE_ADDRESS_MODE_WRAP;
            desc.address_v = AGPU_TEXTURE_ADDRESS_MODE_WRAP;
            desc.address_w = AGPU_TEXTURE_ADDRESS_MODE_WRAP;
            desc.filter = AGPU_FILTER_MIN_LINEAR_MAG_LINEAR_MIPMAP_NEAREST;
            desc.max_lod = 32;

            linearSampler = device->createSampler(&desc);
            if(!linearSampler)
            {
                fprintf(stderr, "Failed to create linear sampler.\n");
                return false;
            }

            guiSamplerBindings->bindSampler(0, linearSampler);
        }

            {
            agpu_sampler_description desc = {};
            desc.address_u = AGPU_TEXTURE_ADDRESS_MODE_WRAP;
            desc.address_v = AGPU_TEXTURE_ADDRESS_MODE_WRAP;
            desc.address_w = AGPU_TEXTURE_ADDRESS_MODE_WRAP;
            desc.filter = AGPU_FILTER_MIN_NEAREST_MAG_NEAREST_MIPMAP_NEAREST;
            desc.max_lod = 32;

            nearestSampler = device->createSampler(&desc);
            if(!nearestSampler)
            {
                fprintf(stderr, "Failed to create nearest sampler.\n");
                return false;
            }

            guiSamplerBindings->bindSampler(1, nearestSampler);
        }
    }

    // GUI empty texture
    {
        guiEmptyTextureBinding = guiShaderSignature->createShaderResourceBinding(2);

        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = 1;
        desc.height = 1;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = AGPU_TEXTURE_FORMAT_B8G8R8A8_UNORM;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_COPY_DESTINATION | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
        desc.sample_count = 1;
        desc.sample_quality = 0;

        whiteTexture = device->createTexture(&desc);
        if(!whiteTexture)
        {
            fprintf(stderr, "Failed to load empty texture.\n");
            return false;
        }
        
        uint32_t color = 0xffffffff;
        whiteTexture->uploadTextureData(0, 0, 4, 4, &color);

        guiEmptyTextureBinding->bindSampledTextureView(0, whiteTexture->getOrCreateFullView());
    }

    // Neutral normal texture
    {
        agpu_texture_description desc = {};
        desc.type = AGPU_TEXTURE_2D;
        desc.width = 1;
        desc.height = 1;
        desc.depth = 1;
        desc.layers = 1;
        desc.miplevels = 1;
        desc.format = AGPU_TEXTURE_FORMAT_B8G8R8A8_UNORM;
        desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_COPY_DESTINATION | AGPU_TEXTURE_USAGE_SAMPLED);
        desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
        desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
        desc.sample_count = 1;
        desc.sample_quality = 0;

        neutralNormalTexture = device->createTexture(&desc);
        if(!neutralNormalTexture)
        {
            fprintf(stderr, "Failed to create neutral normal texture texture.\n");
            return false;
        }
        
        uint32_t color = 0xff8080ff;
        neutralNormalTexture->uploadTextureData(0, 0, 4, 4, &color);
    }

    // Create the GUI pipeline state
    {
        auto vertexShader = compileShader("assets/shaders/GuiShaderCommon.glsl", "assets/shaders/GuiVertexShader.glsl", AGPU_VERTEX_SHADER);
        auto fragmentShader = compileShader("assets/shaders/GuiShaderCommon.glsl", "assets/shaders/GuiFragmentShader.glsl", AGPU_FRAGMENT_SHADER);
        if(!vertexShader || !fragmentShader)
            return false;
        
        auto builder = device->createPipelineBuilder();
        builder->setRenderTargetFormat(0, AGPU_TEXTURE_FORMAT_B8G8R8A8_UNORM_SRGB);
        builder->setDepthStencilFormat(AGPU_TEXTURE_FORMAT_UNKNOWN);

        builder->setShaderSignature(guiShaderSignature);
        builder->attachShader(vertexShader);
        builder->attachShader(fragmentShader);

        builder->setPrimitiveType(AGPU_TRIANGLE_STRIP);
        builder->setBlendState(-1, true);
        builder->setBlendFunction(-1,
            AGPU_BLENDING_ONE, AGPU_BLENDING_INVERTED_SRC_ALPHA, AGPU_BLENDING_OPERATION_ADD,
            AGPU_BLENDING_ONE, AGPU_BLENDING_INVERTED_SRC_ALPHA, AGPU_BLENDING_OPERATION_ADD);

        guiPipelineState = builder->build();
        if(!guiPipelineState)
        {
            fprintf(stderr, "Failed to create GUI pipeline state.");
            return false;
        }

    }

    return true;
}

bool RenderingContext::loadFonts()
{
    defaultFont = Assets::Font::loadFromFile("assets/fonts/DejaVuSans.ttf");
    if(!defaultFont)
    {
        fprintf(stderr, "Failed to load the default font.");
        return false;
    }
    defaultFontFace = defaultFont->getOrCreateFaceWithHeight(16);

    defaultMonospacedFont = Assets::Font::loadFromFile("assets/fonts/DejaVuSansMono.ttf");
    if(!defaultMonospacedFont)
    {
        fprintf(stderr, "Failed to load the default monospaced font.");
        return false;
    }
    defaultMonospacedFontFace = defaultMonospacedFont->getOrCreateFaceWithHeight(16);

    return true;
}

bool RenderingContext::createScenePipelineStates()
{
    // Create the scene shader signature.
    {
        auto shaderSignatureBuilder = device->createShaderSignatureBuilder();

        shaderSignatureBuilder->beginBindingBank(1);
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLER, 1); // Linear
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLER, 1); // Nearest

        // Rendering states.
        shaderSignatureBuilder->beginBindingBank(128);
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_STORAGE_BUFFER, 1); // 0; Object
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_STORAGE_BUFFER, 1); // 1; Camera

        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_UNIFORM_BUFFER, 1); // 2: Global lighting state
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_STORAGE_BUFFER, 1); // 3: World space Lights
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_STORAGE_BUFFER, 1); // 4: View space lights
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_STORAGE_BUFFER, 1); // 5: LightClustersBlock
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_STORAGE_BUFFER, 1); // 6: Tile Light Index List
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_STORAGE_BUFFER, 1); // 7: Light grid Index List

        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // Shadow Map atlas texture
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // BRDF LUT
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // IBL Diffuse Light Probe
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // IBL Specular Light Probe

        // Material state
        shaderSignatureBuilder->beginBindingBank(1024);
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_UNIFORM_BUFFER, 1); // Material state

        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // baseColorTexture
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // emissiveTexture
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // normalTexture
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // occlusionTexture
        shaderSignatureBuilder->addBindingBankElement(AGPU_SHADER_BINDING_TYPE_SAMPLED_IMAGE, 1); // metallicRoughnessTexture

        shaderSignatureBuilder->addBindingConstant(); // Object index
        shaderSignatureBuilder->addBindingConstant(); // Camera index
        shaderSignatureBuilder->addBindingConstant(); // Material index

        sceneShaderSignature = shaderSignatureBuilder->build();
        if (!sceneShaderSignature)
            return false;
    }

    // Create the depth stencil render pass
    {
        agpu_renderpass_depth_stencil_description depthStencilAttachment = {};
        depthStencilAttachment.format = DepthStencilBufferViewFormat;
        depthStencilAttachment.begin_action = AGPU_ATTACHMENT_CLEAR;
        depthStencilAttachment.end_action = AGPU_ATTACHMENT_KEEP;
        depthStencilAttachment.stencil_begin_action = AGPU_ATTACHMENT_CLEAR;
        depthStencilAttachment.stencil_end_action = AGPU_ATTACHMENT_KEEP;

        depthStencilAttachment.sample_count = 1;

        agpu_renderpass_description description = {};
        description.depth_stencil_attachment = &depthStencilAttachment;

        depthStencilRenderPass = device->createRenderPass(&description);
        if(!depthStencilRenderPass)
            return false;
    }
    {
        agpu_renderpass_depth_stencil_description depthStencilAttachment = {};
        depthStencilAttachment.format = DepthStencilBufferViewFormat;
        depthStencilAttachment.begin_action = AGPU_ATTACHMENT_KEEP;
        depthStencilAttachment.end_action = AGPU_ATTACHMENT_KEEP;
        depthStencilAttachment.stencil_begin_action = AGPU_ATTACHMENT_KEEP;
        depthStencilAttachment.stencil_end_action = AGPU_ATTACHMENT_KEEP;
        depthStencilAttachment.sample_count = 1;

        // Color attachment
        agpu_renderpass_color_attachment_description colorAttachment[3] = {};
        colorAttachment[0].format = HDRColorBufferFormat;
        colorAttachment[0].begin_action = AGPU_ATTACHMENT_CLEAR;
        colorAttachment[0].end_action = AGPU_ATTACHMENT_KEEP;
        colorAttachment[0].sample_count = 1;

        colorAttachment[1].format = NormalGBufferFormat;
        colorAttachment[1].begin_action = AGPU_ATTACHMENT_CLEAR;
        colorAttachment[1].end_action = AGPU_ATTACHMENT_KEEP;
        colorAttachment[1].sample_count = 1;

        colorAttachment[2].format = SpecularGBufferFormat;
        colorAttachment[2].begin_action = AGPU_ATTACHMENT_CLEAR;
        colorAttachment[2].end_action = AGPU_ATTACHMENT_KEEP;
        colorAttachment[2].sample_count = 1;

        agpu_renderpass_description description = {};
        description.depth_stencil_attachment = &depthStencilAttachment;
        description.color_attachment_count = 3;
        description.color_attachments = colorAttachment;

        hdrOpaqueRenderPass = device->createRenderPass(&description);
        if(!hdrOpaqueRenderPass)
            return false;
    }

    // Static vertex layout
    {
        agpu_size vertexStrides[] = {
            12, // Position
            12, // Normal 
            8,  // Texcoord
            16, // Tangent4
        };

        agpu_vertex_attrib_description vertexAttributes[] = {
            {0, VertexBufferAttribute::PositionLocation,  AGPU_TEXTURE_FORMAT_R32G32B32_FLOAT,    0, 0},
            {1, VertexBufferAttribute::NormalLocation,    AGPU_TEXTURE_FORMAT_R32G32B32_FLOAT,    0, 0},
            {2, VertexBufferAttribute::Texcoord0Location, AGPU_TEXTURE_FORMAT_R32G32_FLOAT,       0, 0},
            {3, VertexBufferAttribute::Tangent4Location,  AGPU_TEXTURE_FORMAT_R32G32B32A32_FLOAT, 0, 0},
        };

        staticVertexLayout = device->createVertexLayout();
        staticVertexLayout->addVertexAttributeBindings(
            sizeof(vertexStrides)/sizeof(vertexStrides[0]), vertexStrides,
            sizeof(vertexAttributes)/sizeof(vertexAttributes[0]), vertexAttributes
        );
    }

    // Create the depth only pipeline state
    {
        auto vertexShader = compileShader("assets/shaders/SceneShaderCommon.glsl", "assets/shaders/DepthOnlySceneVertexShader.glsl", AGPU_VERTEX_SHADER);
        if(!vertexShader)
            return false;
        
        auto builder = device->createPipelineBuilder();
        builder->setRenderTargetCount(0);
        builder->setDepthStencilFormat(DepthStencilBufferViewFormat);
        builder->setDepthState(true, true, AGPU_GREATER_EQUAL);

        builder->setShaderSignature(sceneShaderSignature);
        builder->attachShader(vertexShader);

        builder->setPrimitiveType(AGPU_TRIANGLES);
        builder->setVertexLayout(staticVertexLayout);

        depthOnlyScenePipelineState = builder->build();
        if(!depthOnlyScenePipelineState)
        {
            fprintf(stderr, "Failed to create depth only scene pipeline state.");
            return false;
        }
    }

    {
        auto computeShader = compileShader("assets/shaders/SceneShaderCommon.glsl", "assets/shaders/TransformLightsToView.glsl", AGPU_COMPUTE_SHADER);
        if(!computeShader)
            return false;
        
        auto builder = device->createComputePipelineBuilder();
        builder->setShaderSignature(sceneShaderSignature);
        builder->attachShader(computeShader);

        transformLightsToViewPipeline = builder->build();
        if(!transformLightsToViewPipeline)
        {
            fprintf(stderr, "Failed to create transform lights to view compute state.");
            return false;
        }
    }

    {
        auto computeShader = compileShader("assets/shaders/SceneShaderCommon.glsl", "assets/shaders/LightGridComputation.glsl", AGPU_COMPUTE_SHADER);
        if(!computeShader)
            return false;
        
        auto builder = device->createComputePipelineBuilder();
        builder->setShaderSignature(sceneShaderSignature);
        builder->attachShader(computeShader);

        lightGridComputationPipeline = builder->build();
        if(!lightGridComputationPipeline)
        {
            fprintf(stderr, "Failed to create light grid computation state.");
            return false;
        }
    }

    {
        auto computeShader = compileShader("assets/shaders/SceneShaderCommon.glsl", "assets/shaders/LightClusterBeginComputation.glsl", AGPU_COMPUTE_SHADER);
        if(!computeShader)
            return false;
        
        auto builder = device->createComputePipelineBuilder();
        builder->setShaderSignature(sceneShaderSignature);
        builder->attachShader(computeShader);

        lightClusterBeginComputationPipeline = builder->build();
        if(!lightClusterBeginComputationPipeline)
        {
            fprintf(stderr, "Failed to create light grid cluster begin pipeline state.");
            return false;
        }
    }

    {
        auto computeShader = compileShader("assets/shaders/SceneShaderCommon.glsl", "assets/shaders/LightClusterListComputation.glsl", AGPU_COMPUTE_SHADER);
        if(!computeShader)
            return false;
        
        auto builder = device->createComputePipelineBuilder();
        builder->setShaderSignature(sceneShaderSignature);
        builder->attachShader(computeShader);

        lightClusterListComputationPipeline = builder->build();
        if(!lightClusterListComputationPipeline)
        {
            fprintf(stderr, "Failed to create light grid cluster list pipeline state.");
            return false;
        }
    }

    // Create the static opaque pipeline state
    {
        auto vertexShader = compileShader("assets/shaders/SceneShaderCommon.glsl", "assets/shaders/StaticSceneVertexShader.glsl", AGPU_VERTEX_SHADER);
        auto fragmentShader = compileShader("assets/shaders/SceneShaderCommon.glsl", "assets/shaders/SceneFragmentShaderCommon.glsl", "assets/shaders/OpaqueFragmentShader.glsl", AGPU_FRAGMENT_SHADER);
        if(!vertexShader || !fragmentShader)
            return false;
        
        auto builder = device->createPipelineBuilder();
        builder->setRenderTargetCount(3);
        builder->setRenderTargetFormat(0, HDRColorBufferFormat);
        builder->setRenderTargetFormat(1, NormalGBufferFormat);
        builder->setRenderTargetFormat(2, SpecularGBufferFormat);
        builder->setDepthStencilFormat(DepthStencilBufferViewFormat);
        builder->setDepthState(true, true, AGPU_EQUAL);

        builder->setShaderSignature(sceneShaderSignature);
        builder->attachShader(vertexShader);
        builder->attachShader(fragmentShader);

        builder->setPrimitiveType(AGPU_TRIANGLES);
        builder->setVertexLayout(staticVertexLayout);

        metallicRoughnessOpaqueScenePipelineState = builder->build();
        if(!metallicRoughnessOpaqueScenePipelineState)
        {
            fprintf(stderr, "Failed to create static opaque pipeline state.");
            return false;
        }
    }

    return true;
}

} // End of namespace Rendering
} // End of namespace Woden