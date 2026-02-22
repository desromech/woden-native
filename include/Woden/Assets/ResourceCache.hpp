#ifndef WODEN_ASSETS_RESOURCE_CACHE_HPP
#define WODEN_ASSETS_RESOURCE_CACHE_HPP

#include "Texture.hpp"
#include "Woden/Rendering/Material.hpp"
#include <unordered_map>

namespace Woden
{
namespace Assets
{
typedef std::shared_ptr<class ResourceCache> ResourceCachePtr;

class ResourceCache : public std::enable_shared_from_this<ResourceCache>
{
public:
    static ResourceCachePtr Get();
    static void releaseSingleton();

    bool initialize();

    TexturePtr getOrCreateCheckboardTexture();
    TexturePtr getOrCreateCheckboardNormalTexture();
    Rendering::MaterialPtr getOrCreateCheckboardMaterial();

    ImagePtr getOrLoadImage(const std::string &path, TextureUsageMode usageMode);
    TexturePtr getOrLoadTexture(const std::string &path, TextureUsageMode usageMode);
    Rendering::MaterialPtr getOrLoadMaterial(const std::string &path);


private:
    TexturePtr checkboardTexture;
    TexturePtr checkboardNormalTexture;
    Rendering::MaterialPtr checkboardMaterial;

    std::string materialSearchPath;

    std::unordered_map<std::string, ImageWeakPtr> loadedImages;
    std::unordered_map<std::string, Rendering::MaterialWeakPtr> loadedMaterials;
    std::unordered_map<std::string, TextureWeakPtr> loadedTextures;
};

} // End of namespace Assets
} // End of namespace Woden

#endif //WODEN_ASSETS_RESOURCE_CACHE_HPP