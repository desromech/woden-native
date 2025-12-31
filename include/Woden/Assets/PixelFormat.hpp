#ifndef WODEN_ASSETS_PIXEL_FORMAT_HPP
#define WODEN_ASSETS_PIXEL_FORMAT_HPP

#include "AGPU/agpu.hpp"

namespace Woden
{
namespace Assets
{

/**
 * I am a specific pixel format.
 */
enum class PixelFormat
{
    None = 0,

    R8_Unorm,
    R16_SNorm,
    R16_UNorm,
    R32_Float,
    
    B8G8R8A8_UNorm,
    B8G8R8A8_UNorm_SRGB,
};

agpu_texture_format mapPixelFormat(PixelFormat format);

}// End of namespace Assets
}// End of namespace Woden

#endif //WODEN_ASSETS_PIXEL_FORMAT_HPP