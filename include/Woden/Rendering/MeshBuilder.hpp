#ifndef WODEN_RENDERING_MESH_BUILDER_HPP
#define WODEN_RENDERING_MESH_BUILDER_HPP

#include "Woden/Math/AABox.hpp"
#include "Woden/Math/Vector2.hpp"
#include "Woden/Math/Vector3.hpp"
#include "Woden/Math/Vector4.hpp"
#include "Woden/Math/Matrix3x3.hpp"
#include "Woden/Math/TRSTransform3D.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/Rendering/MeshPrimitive.hpp"
#include "AGPU/agpu.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Woden
{
namespace Assets
{
typedef std::shared_ptr<class BinaryBuffer> BinaryBufferPtr;
typedef std::shared_ptr<class BinaryBufferView> BinaryBufferViewPtr;
typedef std::shared_ptr<class VertexBinding> VertexBindingPtr;
} // End of namespace Assets

namespace Rendering
{
using Math::Scalar;

typedef std::shared_ptr<class Material> MaterialPtr;

struct MeshBuilderPrimitive
{
    MaterialPtr material;
    agpu_primitive_topology topology = AGPU_TRIANGLES;
    uint32_t firstIndex = 0;
    uint32_t indexCount = 0;
};

class MeshBuilder
{
public:
    MeshBuilder &addCubeWithExtent(const Math::Vector3 &extent);
    MeshBuilder &addCubeWithHalfExtent(const Math::Vector3 &halfExtent);
    MeshBuilder &addCubeWithBox(const Math::AABox &box);

    void beginTriangles();
    void beginWithTopology(agpu_primitive_topology topology);
    void beginWithTopologyAndMaterial(agpu_primitive_topology topology, const MaterialPtr &material);
    void finishLastPrimitive();
    
    void addPxyz(Scalar x, Scalar y, Scalar z);
    void addNxyz(Scalar x, Scalar y, Scalar z);
    void addTriangleI012(uint32_t i0, uint32_t i1, uint32_t i2);

    MeshBuilder &generateTexcoordsWithFacePlanarTransformWithScale(const Math::Vector2 &scale);
    MeshBuilder &generateTexcoordsWithFacePlanarTransform(const Math::Matrix3x3 &transform);
    MeshBuilder &generateTangentSpaceFrame();

    MeshBuilder &translateTo(const Math::Vector3 &translation)
    {
        currentTransform.translation = translation;
        return *this;
    }

    MeshBuilder &setMaterial(const MaterialPtr &newMaterial)
    {
        currentMaterial = newMaterial;
        return *this;
    }

    void encodeBufferData();
    std::vector<MeshPrimitivePtr> encodePrimitives();
    RenderablePtr finishMesh();

    uint32_t baseVertex = 0;
    MaterialPtr currentMaterial;
    Assets::BinaryBufferPtr buffer;
    Assets::BinaryBufferViewPtr vertexBufferView;
    Assets::BinaryBufferViewPtr indexBufferView;
    VertexBindingPtr vertexBinding;

    std::vector<Math::CompactVector3> positions;
    std::vector<Math::CompactVector3> normals;
    std::vector<Math::Vector2> texcoords;
    std::vector<Math::Vector4> tangents4;
    std::vector<uint32_t> indices;
    std::vector<MeshBuilderPrimitive> primitives;

    Math::TRSTransform3D currentTransform;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_MESH_BUILDER_HPP