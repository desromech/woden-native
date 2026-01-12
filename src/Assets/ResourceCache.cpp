#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Assets/ResourceLoadingContext.hpp"
#include "Woden/Math/Scalar.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
#include "Woden/Utility/ReadWholeFile.hpp"
#include "Woden/Utility/FileSystem.hpp"
#include <stdlib.h>
#include <stdio.h>

namespace Woden
{
namespace Assets
{

static ResourceCachePtr uniqueInstance;
ResourceCachePtr ResourceCache::Get()
{
    if(!uniqueInstance)
    {
        uniqueInstance = std::make_shared<ResourceCache> ();
    }

    return uniqueInstance;
}

void ResourceCache::releaseSingleton()
{
    uniqueInstance.reset();
}

bool ResourceCache::initialize()
{
    return true;
}

TexturePtr ResourceCache::getOrCreateCheckboardTexture()
{
    if(checkboardTexture)
        return checkboardTexture;

    auto image = std::make_shared<Woden::Assets::Image> ();
    image->width = 128;
    image->height = 128;
    image->pitch = 4*image->width;
    image->format = Woden::Assets::PixelFormat::B8G8R8A8_UNorm_SRGB;
    image->pixels.resize(image->pitch*image->height);

    image->renderNormalizedPixel32([](Math::Scalar x, Math::Scalar y){
        auto xp = Math::floorFractionPart(x*5.0) < 0.5;
        auto yp = Math::floorFractionPart(y*5.0) < 0.5;
        return (xp ^ yp) ? 0xffffffff : 0xff000000;
    });

    checkboardTexture = image->asTexture();
    checkboardTexture->usageMode = TextureUsageMode::Color;
    checkboardTexture->generateMipmaps();
    return checkboardTexture;
}

TexturePtr ResourceCache::getOrCreateCheckboardNormalTexture()
{
    if(checkboardNormalTexture)
        return checkboardNormalTexture;
    
    auto normalImage = checkboardTexture->miplevels[0]->intoNormalMap();
    checkboardNormalTexture = normalImage->asTexture();
    checkboardNormalTexture->usageMode = TextureUsageMode::Normal;
    checkboardNormalTexture->generateMipmaps();
    return checkboardNormalTexture;
}

Rendering::MaterialPtr ResourceCache::getOrCreateCheckboardMaterial()
{
    if(!checkboardMaterial)
    {
        auto material = std::make_shared<Woden::Rendering::MetallicRoughnessMaterial> ();
        material->roughnessFactor = 0.4;
        material->metallicFactor = 0.0;
        material->baseColorTexture = getOrCreateCheckboardTexture();
        material->normalTexture = getOrCreateCheckboardNormalTexture();
        checkboardMaterial = material;
    }

    return checkboardMaterial;
}

TexturePtr ResourceCache::getOrLoadTexture(const std::string &path, TextureUsageMode usageMode)
{
    auto it = loadedTextures.find(path);
    if(it != loadedTextures.end())
    {
        auto existent = it->second.lock();
        if(existent)
            return existent;
    }

    printf("TODO: load texture %s\n", path.c_str());
    return nullptr;
}

Rendering::MaterialPtr ResourceCache::getOrLoadMaterial(const std::string &path)
{
    auto it = loadedMaterials.find(path);
    if(it != loadedMaterials.end())
    {
        auto existent = it->second.lock();
        if(existent)
            return existent;
    }

    auto materialText = Utility::readWholeTextFile(path);
    if(materialText.empty())
    {
        fprintf(stderr, "Failed to load material %s\n", path.c_str());
        return nullptr;
    }

    rapidjson::Document document;
    document.Parse(materialText.c_str());
    if(!document.IsObject())
    {
        fprintf(stderr, "Failed to load material %s: expected a json object\n", path.c_str());
        return nullptr;
    }

    if(!document.HasMember("type") || !document["type"].IsString())
    {
        fprintf(stderr, "Failed to load material %s: expected a string with the material type\n", path.c_str());
        return nullptr;

    }

    std::string typeName = document["type"].GetString();

    auto materialFactory = Rendering::MaterialFactoryRegistry::Get()->findFactory(typeName);
    if(!materialFactory)
    {
        fprintf(stderr, "Failed to load material %s: unsupported material type %s\n", path.c_str(), typeName.c_str());
        return nullptr;

    }

    auto material = materialFactory->makeInstance();
    assert(material);

    ResourceLoadingContext context;
    context.jsonObject = document.GetObj();
    context.directory = Utility::dirname(path);
    context.resourceCache = shared_from_this();

    material->loadWithContext(context);

    return material;
}


} // End of namespace Assets
} // End of namespace Woden
