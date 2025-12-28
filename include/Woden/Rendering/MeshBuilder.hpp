#ifndef WODEN_RENDERING_MESH_BUILDER_HPP
#define WODEN_RENDERING_MESH_BUILDER_HPP

#include "Woden/Math/AABox.hpp"
#include "Woden/Math/Vector2.hpp"
#include "Woden/Math/Vector3.hpp"
#include "Woden/Math/Vector4.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Woden
{
namespace Rendering
{
using Math::Scalar;

class MeshBuilder
{
public:
    void addCubeWithExtent(const Math::Vector3 &extent);
    void addCubeWithHalfExtent(const Math::Vector3 &halfExtent);
    void addCubeWithBox(const Math::AABox &box);

    void beginTriangles();
    void addPxyz(Scalar x, Scalar y, Scalar z);
    void addNxyz(Scalar x, Scalar y, Scalar z);
    void addTriangleI012(uint32_t i0, uint32_t i1, uint32_t i2);

    RenderablePtr finishMesh();

    uint32_t baseVertex = 0;

    std::vector<Math::CompactVector3> positions;
    std::vector<Math::Vector4> colors;
    std::vector<Math::Vector2> texcoords;
    std::vector<Math::CompactVector3> normals;
    std::vector<Math::Vector4> tangents4;
    std::vector<uint32_t> indices;
};

} // End of namespace Rendering
} // End of namespace Woden
#endif //WODEN_RENDERING_MESH_BUILDER_HPP