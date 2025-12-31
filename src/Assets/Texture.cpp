#include "Woden/Assets/Texture.hpp"
#include "Woden/Rendering/Context.hpp"

namespace Woden
{
namespace Assets
{
agpu_texture_ref Texture::getValidTextureHandle()
{
    if(textureHandle)
        return textureHandle;
    
    const auto &topMipLevel = miplevels.front();

    agpu_texture_description desc = {};
    desc.type = AGPU_TEXTURE_2D;
    desc.width = topMipLevel->width;
    desc.height = topMipLevel->height;
    desc.depth = 1;
    desc.layers = 1;
    desc.miplevels = miplevels.size();
    desc.format = mapPixelFormat(topMipLevel->format);
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

    for(size_t levelIndex = 0; levelIndex < miplevels.size(); ++levelIndex)
    {
        const auto &miplevel = miplevels[levelIndex];
        textureHandle->uploadTextureData(levelIndex, 0, miplevel->pitch, miplevel->pitch*miplevel->height, miplevel->pixels.data());

    }

    return textureHandle;
}

} // End of namespace Assets
} // End of namespace Woden