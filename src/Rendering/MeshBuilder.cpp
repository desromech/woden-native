#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/StaticMeshRenderable.hpp"

namespace Woden
{
namespace Rendering
{

void MeshBuilder::addCubeWithExtent(const Math::Vector3 &extent)
{
    addCubeWithHalfExtent(extent * 0.5);
}

void MeshBuilder::addCubeWithHalfExtent(const Math::Vector3 &halfExtent)
{
    addCubeWithBox(Math::AABox(-halfExtent, halfExtent));
}

void MeshBuilder::addCubeWithBox(const Math::AABox &box)
{
    auto minX = box.minCorner.x;
    auto minY = box.minCorner.y;
    auto minZ = box.minCorner.z;

    auto maxX = box.maxCorner.x;
    auto maxY = box.maxCorner.y;
    auto maxZ = box.maxCorner.z;

    // Left
    beginTriangles();
    addPxyz(minX, minY, minZ); addNxyz(-1.0, 0.0, 0.0);
    addPxyz(minX, maxY, minZ); addNxyz(-1.0, 0.0, 0.0);
    addPxyz(minX, maxY, maxZ); addNxyz(-1.0, 0.0, 0.0);
    addPxyz(minX, minY, maxZ); addNxyz(-1.0, 0.0, 0.0);
    addTriangleI012(1, 0, 2);
    addTriangleI012(3, 2, 0);

    /*self
    "Left"

    "Right"
    beginTriangles;
    addPx: maxX y: minY z: minZ; addNx: 1.0 y: 0.0 z: 0.0;
    addPx: maxX y: maxY z: minZ; addNx: 1.0 y: 0.0 z: 0.0;
    addPx: maxX y: maxY z: maxZ; addNx: 1.0 y: 0.0 z: 0.0;
    addPx: maxX y: minY z: maxZ; addNx: 1.0 y: 0.0 z: 0.0;
    addTriangleI0: 0 i1: 1 i2: 2;
    addTriangleI0: 2 i1: 3 i2: 0;

    "Top"
    beginTriangles;
    addPx: minX y: maxY z: minZ; addNx: 0.0 y: 1.0 z: 0.0;
    addPx: maxX y: maxY z: minZ; addNx: 0.0 y: 1.0 z: 0.0;
    addPx: maxX y: maxY z: maxZ; addNx: 0.0 y: 1.0 z: 0.0;
    addPx: minX y: maxY z: maxZ; addNx: 0.0 y: 1.0 z: 0.0;
    addTriangleI0: 1 i1: 0 i2: 2;
    addTriangleI0: 3 i1: 2 i2: 0;

    "Bottom"
    beginTriangles;
    addPx: minX y: minY z: minZ; addNx: 0.0 y: -1.0 z: 0.0;
    addPx: maxX y: minY z: minZ; addNx: 0.0 y: -1.0 z: 0.0;
    addPx: maxX y: minY z: maxZ; addNx: 0.0 y: -1.0 z: 0.0;
    addPx: minX y: minY z: maxZ; addNx: 0.0 y: -1.0 z: 0.0;
    addTriangleI0: 0 i1: 1 i2: 2;
    addTriangleI0: 2 i1: 3 i2: 0;

    "Back"
    beginTriangles;
    addPx: minX y: minY z: minZ; addNx: 0.0 y: 0.0 z: -1.0;
    addPx: maxX y: minY z: minZ; addNx: 0.0 y: 0.0 z: -1.0;
    addPx: maxX y: maxY z: minZ; addNx: 0.0 y: 0.0 z: -1.0;
    addPx: minX y: maxY z: minZ; addNx: 0.0 y: 0.0 z: -1.0;
    addTriangleI0: 1 i1: 0 i2: 2;
    addTriangleI0: 3 i1: 2 i2: 0;

    "Front"
    beginTriangles;
    addPx: minX y: minY z: maxZ; addNx: 0.0 y: 0.0 z: 1.0;
    addPx: maxX y: minY z: maxZ; addNx: 0.0 y: 0.0 z: 1.0;
    addPx: maxX y: maxY z: maxZ; addNx: 0.0 y: 0.0 z: 1.0;
    addPx: minX y: maxY z: maxZ; addNx: 0.0 y: 0.0 z: 1.0;
    addTriangleI0: 0 i1: 1 i2: 2;
    addTriangleI0: 2 i1: 3 i2: 0*/
}

void MeshBuilder::beginTriangles()
{
    baseVertex = positions.size();
}

void MeshBuilder::addPxyz(Scalar x, Scalar y, Scalar z)
{
    positions.push_back(Woden::Math::CompactVector3(x, y, z));
}
void MeshBuilder::addNxyz(Scalar x, Scalar y, Scalar z)
{
    normals.push_back(Woden::Math::CompactVector3(x, y, z));

}
void MeshBuilder::addTriangleI012(uint32_t i0, uint32_t i1, uint32_t i2)
{
    indices.push_back(i0);
    indices.push_back(i1);
    indices.push_back(i2);
}

RenderablePtr MeshBuilder::finishMesh()
{
    auto mesh = std::make_shared<StaticMeshRenderable> ();
    return mesh;
}

} // End of namespace Rendering
} // End of namespace Woden