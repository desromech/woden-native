#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Math/Scalar.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
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
    checkboardTexture->generateColorMipmaps();
    return checkboardTexture;
}

TexturePtr ResourceCache::getOrCreateCheckboardNormalTexture()
{
    if(checkboardNormalTexture)
        return checkboardNormalTexture;
    
    auto normalImage = checkboardTexture->miplevels[0]->intoNormalMap();
    checkboardNormalTexture = normalImage->asTexture();
    checkboardNormalTexture->generateDataMipmaps();
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


} // End of namespace Assets
} // End of namespace Woden
