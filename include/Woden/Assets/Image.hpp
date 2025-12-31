#ifndef WODEN_ASSETS_IMAGE_HPP
#define WODEN_ASSETS_IMAGE_HPP

#include "PixelFormat.hpp"
#include <stdint.h>
#include <string>
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

    template<typename FT>
    void renderPixels32(FT &&renderFunction)
    {
        auto destinationRow = pixels.data();
        for(uint32_t y = 0; y < height; ++y)
        {
            auto currentRow = reinterpret_cast<uint32_t*> (destinationRow);
            for(uint32_t x = 0; x < width; ++x)
                currentRow[x] = renderFunction(x, y, width, height);
            destinationRow += pitch;
        }
    }

    bool saveToTGA(const std::string &filename);

    agpu_texture_ref getValidTextureHandle();
    agpu_shader_resource_binding_ref getValidGuiTextureBinding();

    agpu_texture_ref textureHandle;
    agpu_shader_resource_binding_ref guiTextureBinding;
};

}// End of namespace Assets
}// End of namespace Woden

#endif //WODEN_ASSETS_IMAGE_HPP