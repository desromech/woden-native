#ifndef WODEN_ASSETS_IMAGE_HPP
#define WODEN_ASSETS_IMAGE_HPP

#include "PixelFormat.hpp"

#include <stdint.h>
#include <memory>
#include <vector>

namespace Woden
{
namespace Assets
{

typedef std::shared_ptr<class Image> ImagePtr;

class Image
{
public:
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t pitch = 0;
    PixelFormat format = PixelFormat::None;

    std::vector<uint8_t> pixels;
};

}// End of namespace Assets
}// End of namespace Woden

#endif //WODEN_ASSETS_IMAGE_HPP