#ifndef WODEN_STATIC_MESH_PRIMITIVE_HPP
#define WODEN_STATIC_MESH_PRIMITIVE_HPP

#include "Woden/Rendering/Renderable.hpp"
#include <vector>

namespace Woden
{
namespace Rendering
{
typedef std::shared_ptr<class StaticMeshRenderable> StaticMeshRenderablePtr;
typedef std::shared_ptr<class MeshPrimitive> MeshPrimitivePtr;

class StaticMeshRenderable : public Renderable
{
public:

    std::vector<MeshPrimitivePtr> primitives;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_STATIC_MESH_PRIMITIVE_HPP