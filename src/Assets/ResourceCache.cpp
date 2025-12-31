#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Math/Scalar.hpp"
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
    return checkboardTexture;
}

TexturePtr ResourceCache::getOrCreateCheckboardNormalTexture()
{
    if(checkboardNormalTexture)
        return checkboardNormalTexture;
    return checkboardNormalTexture;
}


} // End of namespace Assets
} // End of namespace Woden