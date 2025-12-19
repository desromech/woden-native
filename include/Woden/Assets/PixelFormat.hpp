#ifndef WODEN_ASSETS_PIXEL_FORMAT_HPP
#define WODEN_ASSETS_PIXEL_FORMAT_HPP

namespace Woden
{
namespace Assets
{

enum class PixelFormat
{
    None = 0,

    R8_Unorm,
    R16_SNorm,
    R16_UNorm,
    R32_Float,
    
    B8G8R8A8_UNorm,
};

}// End of namespace Assets
}// End of namespace Woden

#endif //WODEN_ASSETS_PIXEL_FORMAT_HPP