#include "Woden/Assets/Image.hpp"
#include "Woden/Assets/Texture.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Math/Vector3.hpp"
#include <stdio.h>

namespace Woden
{
namespace Assets
{

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

float Image::fetchHeight(int x, int y)
{
    x %= width;
    y %= height;

    auto row = pixels.data() + pitch*y;
    auto row32 = reinterpret_cast<uint32_t*> (row);
    auto pixelValue = row32[x];

    auto height = (pixelValue & 0xff) / 255.0f;
    return height;
}

ImagePtr Image::intoNormalMap()
{
    auto normalImage = std::make_shared<Image> ();
    normalImage->width = width;
    normalImage->height = height;
    normalImage->pitch = normalImage->width*4;
    normalImage->format = PixelFormat::B8G8R8A8_UNorm;
    normalImage->pixels.resize(normalImage->pitch*normalImage->height);

    normalImage->renderPixels32([&](int x, int y, int width, int height){
        auto dx = (fetchHeight(x + 1, y) - fetchHeight(x - 1, y)) *0.5f;
        auto dy = (fetchHeight(x, y + 1) - fetchHeight(x, y - 1)) *0.5f;

        auto N = Math::Vector3(-dx, -dy, 1);
        auto normalizedN = N.normalized();
        auto pixelValue = normalizedN*0.5 + 0.5;

        auto cr = uint32_t(pixelValue.x *255);
        auto cg = uint32_t(pixelValue.y *255);
        auto cb = uint32_t(pixelValue.z *255);
        return cb | (cg << 8) | (cr << 16) | 0xff000000;
    });

    return normalImage;
}

TexturePtr Image::asTexture()
{
    auto texture = std::make_shared<Texture> ();
    texture->miplevels.push_back(shared_from_this());
    return texture;
}


}// End of namespace Assets
}// End of namespace Woden
