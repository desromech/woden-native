#include "Woden/Assets/Texture.hpp"

namespace Woden
{
namespace Assets
{
agpu_texture_ref Texture::getValidTextureHandle()
{
    if(textureHandle)
        return textureHandle;

    abort();
    return textureHandle;
}

} // End of namespace Assets
} // End of namespace Woden