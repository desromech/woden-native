#include "Woden/Rendering/Material.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"

namespace Woden
{
namespace Rendering
{

static MaterialFactoryRegistry materialFactoryRegistry;
MaterialFactoryRegistry *MaterialFactoryRegistry::Get()
{
    return &materialFactoryRegistry;
}

static MaterialPtr defaultMaterial;

MaterialPtr Material::getDefaultMaterial()
{
    if(!defaultMaterial)
    {
        auto metallicRoughness = std::make_shared<MetallicRoughnessMaterial> ();
        metallicRoughness->roughnessFactor = 0.4;
        metallicRoughness->metallicFactor = 0;
        defaultMaterial = metallicRoughness;
    }

    return defaultMaterial;
}

} // End of namespace Rendering
} // End of namespace Woden