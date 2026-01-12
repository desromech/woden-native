#ifndef WODEN_RESOURCE_LOADING_CONTEXT_HPP
#define WODEN_RESOURCE_LOADING_CONTEXT_HPP

#include "Texture.hpp"
#include "Woden/Math/Vector2.hpp"
#include "Woden/Math/Vector3.hpp"
#include "Woden/Math/Vector4.hpp"
#include "rapidjson/document.h"
#include <string>

namespace Woden
{
namespace Assets
{
typedef std::shared_ptr<class ResourceCache> ResourceCachePtr;

/**
 * I am a context which is used to faciliate the loading of resources.
 */
class ResourceLoadingContext
{
public:
    rapidjson::Value jsonObject;
    Assets::ResourceCachePtr resourceCache;
    std::string directory;

    TexturePtr getOrLoadTexture(const std::string &path, TextureUsageMode usageMode);
    TexturePtr getTextureMember(const char *member, TextureUsageMode usageMode);

    bool getBool(const char *member, bool defaultValue)
    {
        if(!jsonObject.HasMember(member))
            return defaultValue;
        return jsonObject[member].GetBool();
    }

    float getFloat(const char *member, float defaultValue)
    {
        if(!jsonObject.HasMember(member))
            return defaultValue;
        return jsonObject[member].GetFloat();
    }


    Math::Vector2 getVector2(const char *member, Math::Vector2 defaultValue)
    {
        if(jsonObject.HasMember(member))
        {
            auto &array = jsonObject[member];
            if(array.IsArray() && array.Size() == 2)
                return Math::Vector2(array[0].GetFloat(), array[1].GetFloat());
        }
        return defaultValue;
    }

    Math::Vector3 getVector3(const char *member, Math::Vector3 defaultValue)
    {
        if(jsonObject.HasMember(member))
        {
            auto &array = jsonObject[member];
            if(array.IsArray() && array.Size() == 3)
                return Math::Vector3(array[0].GetFloat(), array[1].GetFloat(), array[2].GetFloat());
        }
        return defaultValue;
    }

    Math::Vector4 getVector4(const char *member, Math::Vector4 defaultValue)
    {
        if(jsonObject.HasMember(member))
        {
            auto &array = jsonObject[member];
            if(array.IsArray() && array.Size() == 4)
                return Math::Vector4(array[0].GetFloat(), array[1].GetFloat(), array[2].GetFloat(), array[3].GetFloat());
        }
        return defaultValue;
    }
};

}
}

#endif //WODEN_RESOURCE_LOADING_CONTEXT_HPP