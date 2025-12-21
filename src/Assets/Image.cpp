#include "Woden/Assets/Image.hpp"
#include "Woden/Rendering/Context.hpp"
#include <stdio.h>

namespace Woden
{
namespace Assets
{

agpu_texture_format mapPixelFormat(PixelFormat format)
{
    switch(format)
    {
    case PixelFormat::None: return AGPU_TEXTURE_FORMAT_UNKNOWN;

    case PixelFormat::R8_Unorm:  return AGPU_TEXTURE_FORMAT_R8_UNORM;
    case PixelFormat::R16_SNorm: return AGPU_TEXTURE_FORMAT_R16_SNORM;
    case PixelFormat::R16_UNorm: return AGPU_TEXTURE_FORMAT_R16_UNORM;
    case PixelFormat::R32_Float: return AGPU_TEXTURE_FORMAT_R32_FLOAT;
    
    case PixelFormat::B8G8R8A8_UNorm: return AGPU_TEXTURE_FORMAT_B8G8R8A8_UNORM;
    default: abort();
    }
}

#ifdef _WIN32
#define PACKED
#pragma pack(push, 1)
#else
#define PACKED __attribute__((packed))
#endif

struct PACKED TGAHeader
{
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t imageType;

    uint16_t colorMapFirstEntry;
    uint16_t colorMapLengh;
    uint8_t colorMapEntrySize;

    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t pixelDepth;
    uint8_t imageDescriptor;
};

#ifdef _WIN32
#define PACKED
#pragma pack(pop)
#endif

bool Image::saveToTGA(const std::string &filename)
{
    FILE *file = fopen(filename.c_str(), "wb");
    if(!file)
        return false;

    TGAHeader header = {0};
    header.imageType = 2;
    header.width = width;
    header.height = height;
    header.pixelDepth = 32;

    if(1 != fwrite(&header, sizeof(TGAHeader), 1, file))
    {
        perror("Failed to write TGA header");
        fclose(file);
        return false;
    }

    if(1 != fwrite(pixels.data(), pixels.size(), 1, file))
    {
        perror("Failed to write TGA pixels");
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

agpu_texture_ref Image::getValidTextureHandle()
{
    if(textureHandle)
        return textureHandle;

    agpu_texture_description desc = {};
    desc.type = AGPU_TEXTURE_2D;
    desc.width = width;
    desc.height = height;
    desc.depth = 1;
    desc.layers = 1;
    desc.miplevels = 1;
    desc.format = mapPixelFormat(format);
    desc.usage_modes = agpu_texture_usage_mode_mask(AGPU_TEXTURE_USAGE_COPY_DESTINATION | AGPU_TEXTURE_USAGE_SAMPLED);
    desc.main_usage_mode = AGPU_TEXTURE_USAGE_SAMPLED;
    desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
    desc.sample_count = 1;
    desc.sample_quality = 0;

    textureHandle = Rendering::RenderingContext::getMainContext()->device->createTexture(&desc);
    if(!textureHandle)
    {
        fprintf(stderr, "Failed to create image texture.\n");
        return nullptr;
    }
    
    textureHandle->uploadTextureData(0, 0, pitch, pitch*height, pixels.data());
    return textureHandle;
}

agpu_shader_resource_binding_ref Image::getValidGuiTextureBinding()
{
    if(guiTextureBinding)
        return guiTextureBinding;

    if(!getValidTextureHandle())
        return nullptr;

    guiTextureBinding = Rendering::RenderingContext::getMainContext()->guiShaderSignature->createShaderResourceBinding(2);
    guiTextureBinding->bindSampledTextureView(0, textureHandle->getOrCreateFullView());
    return guiTextureBinding;
}


}// End of namespace Assets
}// End of namespace Woden
