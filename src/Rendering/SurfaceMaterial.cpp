#include "Woden/Rendering/SurfaceMaterial.hpp"
#include "Woden/Assets/ResourceLoadingContext.hpp"

namespace Woden
{
namespace Rendering
{
void SurfaceMaterial::loadWithContext(Assets::ResourceLoadingContext &context)
{
    alphaCutoff = context.getFloat("alphaCutoff", alphaCutoff);
    doubleSided = context.getBool("doubleSided", doubleSided);
}

} // End of namespace Rendering
} // End of namespace Woden