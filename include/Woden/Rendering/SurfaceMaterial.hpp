#ifndef WODEN_SURFACE_MATERIAL_HPP
#define WODEN_SURFACE_MATERIAL_HPP

#include "Material.hpp"

namespace Woden
{
namespace Rendering
{

/**
 * I am a specific surface alpha mode.
 */
enum class SurfaceAlphaMode
{
    Opaque,
    Mask,
    Blend,
};

/**
 * I am a metallic-roughness material
 */
class SurfaceMaterial : public Material
{
public:
    SurfaceAlphaMode alphaMode = SurfaceAlphaMode::Opaque;
    float alphaCutoff = 0.5;
    bool doubleSided = false;

};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SURFACE_MATERIAL_HPP
