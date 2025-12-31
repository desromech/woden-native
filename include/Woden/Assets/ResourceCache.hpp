#ifndef WODEN_ASSETS_RESOURCE_CACHE_HPP
#define WODEN_ASSETS_RESOURCE_CACHE_HPP

#include "Texture.hpp"

namespace Woden
{
namespace Assets
{
typedef std::shared_ptr<class ResourceCache> ResourceCachePtr;

class ResourceCache
{
public:
    static ResourceCachePtr Get();
    static void releaseSingleton();

    bool initialize();

    TexturePtr getOrCreateCheckboardTexture();
    TexturePtr getOrCreateCheckboardNormalTexture();

private:
    TexturePtr checkboardTexture;
    TexturePtr checkboardNormalTexture;
};

} // End of namespace Assets
} // End of namespace Woden

#endif //WODEN_ASSETS_RESOURCE_CACHE_HPP