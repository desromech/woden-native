#ifndef WODEN_ASSETS_TEXTURE_HPP
#define WODEN_ASSETS_TEXTURE_HPP

#include "Image.hpp"
#include <vector>

namespace Woden
{
namespace Assets
{
typedef std::shared_ptr<class Texture> TexturePtr;
typedef std::weak_ptr<class Texture> TextureWeakPtr;

enum class TextureUsageMode {
    Color,
    Data,
    Normal,
};

class Texture
{
public:
    agpu_texture_ref getValidTextureHandle();
    void generateMipmaps();
    void generateColorMipmaps();
    void generateNormalMipmaps();
    void generateDataMipmaps();
    
    std::vector<ImagePtr> miplevels;
    agpu_texture_ref textureHandle;
    TextureUsageMode usageMode = TextureUsageMode::Color;

};

} // End of namespace Assets
} // End of namespace Woden

#endif //WODEN_ASSETS_TEXTURE_HPP
