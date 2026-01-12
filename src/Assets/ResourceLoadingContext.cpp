#include "Woden/Assets/ResourceLoadingContext.hpp"
#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Utility/FileSystem.hpp"

namespace Woden
{
namespace Assets
{
TexturePtr ResourceLoadingContext::getOrLoadTexture(const std::string &path, TextureUsageMode usageMode)
{
    std::string fullPath = Utility::join(directory, path);
    return resourceCache->getOrLoadTexture(fullPath, usageMode);
}

TexturePtr ResourceLoadingContext::getTextureMember(const char *member, TextureUsageMode usageMode)
{
    if(jsonObject.HasMember(member) && jsonObject[member].IsString())
        return getOrLoadTexture(jsonObject[member].GetString(), usageMode);
    return nullptr;
}

} // End of namespace Assets
} // End of namespace Woden.
