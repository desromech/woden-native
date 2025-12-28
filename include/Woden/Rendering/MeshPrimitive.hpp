#ifndef WODEN_MESH_PRIMITIVE_HPP
#define WODEN_MESH_PRIMITIVE_HPP

#include "Renderable.hpp"
#include "AGPU/agpu.hpp"
namespace Woden
{
namespace Rendering
{
typedef std::shared_ptr<class Material> MaterialPtr;
typedef std::shared_ptr<class MeshPrimitive> MeshPrimitivePtr;

class MeshPrimitive : public Renderable
{
public:
    MaterialPtr material;
    agpu_primitive_topology topology = AGPU_TRIANGLES;
    uint32_t firstIndex = 0;
    uint32_t indexCount = 0;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_MESH_PRIMITIVE_HPP