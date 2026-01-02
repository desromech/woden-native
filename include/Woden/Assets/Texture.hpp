#ifndef WODEN_ASSETS_TEXTURE_HPP
#define WODEN_ASSETS_TEXTURE_HPP

#include "Image.hpp"
#include <vector>

namespace Woden
{
namespace Assets
{
typedef std::shared_ptr<class Texture> TexturePtr;

class Texture
{
public:
    agpu_texture_ref getValidTextureHandle();
    void generateColorMipmaps();
    void generateDataMipmaps();
    
    std::vector<ImagePtr> miplevels;
    agpu_texture_ref textureHandle;

};

} // End of namespace Assets
} // End of namespace Woden

#endif //WODEN_ASSETS_TEXTURE_HPP
