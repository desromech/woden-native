#include "Woden/Assets/PixelFormat.hpp"

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
    case PixelFormat::B8G8R8A8_UNorm_SRGB: return AGPU_TEXTURE_FORMAT_B8G8R8A8_UNORM_SRGB;
    
    default: abort();
    }
}

}
}
