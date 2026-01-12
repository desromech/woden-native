#include "Woden/Assets/Image.hpp"
#include "Woden/Assets/Texture.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Math/Vector3.hpp"
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

inline uint32_t encodeDataPixel32(const Math::Vector4 &data)
{
    auto r = uint8_t(data.x*255);
    auto g = uint8_t(data.y*255);
    auto b = uint8_t(data.z*255);
    auto a = uint8_t(data.w*255);

    return b | (g << 8) | (r << 16) | (a << 24);
}

ImagePtr Image::loadFromFilenamed(const std::string &filename, TextureUsageMode usageMode)
{
    int x,y,n;
    unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, 4);
    if(!data)
        return nullptr;

    auto image = std::make_shared<Image> ();
    image->width = x;
    image->height = y;
    image->pitch = x*4;
    image->format = usageMode == TextureUsageMode::Color ? PixelFormat::R8G8B8A8_UNorm_SRGB : PixelFormat::R8G8B8A8_UNorm;
    image->pixels.resize(image->pitch*image->height);
    memcpy(image->pixels.data(), data, image->pitch*image->height);

    stbi_image_free(data);

    return image;
}

bool Image::saveToTGA(const std::string &filename)
{
    FILE *file = fopen(filename.c_str(), "wb");
    if(!file)
        return false;

    TGAHeader header = {};
    header.imageType = 2;
    header.width = uint16_t(width);
    header.height = uint16_t(height);
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

    auto heightSample = (pixelValue & 0xff) / 255.0f;
    return heightSample;
}

Math::Vector4 Image::fetchData(int x, int y)
{
    x %= width;
    y %= height;

    auto row = pixels.data() + pitch*y;
    auto row32 = reinterpret_cast<uint32_t*> (row);
    auto pixelValue = row32[x];

    auto b = pixelValue & 0xFF;
    auto g = (pixelValue >> 8) & 0xFF;
    auto r = (pixelValue >> 16) & 0xFF;
    auto a = (pixelValue >> 24) & 0xFF;

    auto nr = r/255.0f;
    auto ng = g/255.0f;
    auto nb = b/255.0f;
    auto na = a/255.0f;
    
    return Math::Vector4(nr, ng, nb, na);
}

Math::Vector4 Image::sampleDataAtTexcoord(Math::Vector2 texcoord)
{
    Math::Vector2 position = texcoord*Math::Vector2(Math::Scalar(width), Math::Scalar(height));

    Math::Vector2 leftTopCoord     = position.floor();
    Math::Vector2 leftBottomCoord  = leftTopCoord + Math::Vector2(0, 1);
    Math::Vector2 rightTopCoord    = leftTopCoord + Math::Vector2(1, 0);
    Math::Vector2 rightBottomCoord = leftTopCoord + Math::Vector2(1, 1);

    Math::Vector2 fractCoord = position - leftTopCoord;

    Math::Vector4 leftTopValue     = fetchData(int(leftTopCoord.x),     int(leftTopCoord.y));
    Math::Vector4 leftBottomValue  = fetchData(int(leftBottomCoord.x),  int(leftBottomCoord.y));
    Math::Vector4 rightTopValue    = fetchData(int(rightTopCoord.x),    int(rightTopCoord.y));
    Math::Vector4 rightBottomValue = fetchData(int(rightBottomCoord.x), int(rightBottomCoord.y));

    auto topValue = Math::mix(leftTopValue, rightTopValue, fractCoord.x);
    auto bottomValue = Math::mix(leftBottomValue, rightBottomValue, fractCoord.x);
    auto value = Math::mix(topValue, bottomValue, fractCoord.y);
    return value;
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
        (void)width;
        (void)height;
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

ImagePtr Image::computeNextNormalMipLevel()
{
    auto nextLevel = std::make_shared<Image> ();
    nextLevel->width = std::max(uint32_t(1), width/2);
    nextLevel->height = std::max(uint32_t(1), height/2);
    nextLevel->pitch = nextLevel->width*4;
    nextLevel->format = format;
    nextLevel->pixels.resize(nextLevel->pitch*nextLevel->height);

    nextLevel->renderPixels32([&](uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        (void)w;
        (void)h;
        auto texcoord = Math::Vector2(x*2 + 0.5f, y*2 + 0.5f);
        texcoord = texcoord / Math::Vector2(Math::Scalar(width), Math::Scalar(height));
        auto mipData = sampleDataAtTexcoord(texcoord);
        
        auto N = (mipData.xyz() * 2 - 1).normalized();
        auto normalizedMipData = Math::Vector4(N*0.5 + 0.5, mipData.w);
        return encodeDataPixel32(normalizedMipData);
    });

    return nextLevel;
}

ImagePtr Image::computeNextDataMipLevel()
{
    auto nextLevel = std::make_shared<Image> ();
    nextLevel->width = std::max(uint32_t(1), width/2);
    nextLevel->height = std::max(uint32_t(1), height/2);
    nextLevel->pitch = nextLevel->width*4;
    nextLevel->format = format;
    nextLevel->pixels.resize(nextLevel->pitch*nextLevel->height);

    nextLevel->renderPixels32([&](uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        (void)w;
        (void)h;
        auto texcoord = Math::Vector2(x*2 + 0.5f, y*2 + 0.5f);
        texcoord = texcoord / Math::Vector2(Math::Scalar(width), Math::Scalar(height));
        auto mipData = sampleDataAtTexcoord(texcoord);
        return encodeDataPixel32(mipData);
    });

    return nextLevel;
}

TexturePtr Image::asTexture()
{
    auto texture = std::make_shared<Texture> ();
    texture->miplevels.push_back(shared_from_this());
    return texture;
}


}// End of namespace Assets
}// End of namespace Woden

