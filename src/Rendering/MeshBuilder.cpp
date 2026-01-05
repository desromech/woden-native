#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/StaticMeshRenderable.hpp"
#include "Woden/Rendering/VertexBinding.hpp"
#include "Woden/Assets/BinaryBuffer.hpp"
#include <assert.h>

namespace Woden
{
namespace Rendering
{

MeshBuilder &MeshBuilder::addCubeWithExtent(const Math::Vector3 &extent)
{
    return addCubeWithHalfExtent(extent * Math::Vector3(0.5));
}

MeshBuilder &MeshBuilder::addCubeWithHalfExtent(const Math::Vector3 &halfExtent)
{
    return addCubeWithBox(Math::AABox(-halfExtent, halfExtent));
}

MeshBuilder &MeshBuilder::addCubeWithBox(const Math::AABox &box)
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

    // Right
    beginTriangles();
    addPxyz(maxX, minY, minZ); addNxyz(1.0, 0.0, 0.0);
    addPxyz(maxX, maxY, minZ); addNxyz(1.0, 0.0, 0.0);
    addPxyz(maxX, maxY, maxZ); addNxyz(1.0, 0.0, 0.0);
    addPxyz(maxX, minY, maxZ); addNxyz(1.0, 0.0, 0.0);
    addTriangleI012(0, 1, 2);
    addTriangleI012(2, 3, 0);

    // Top
    beginTriangles();
    addPxyz(minX, maxY, minZ); addNxyz(0.0, 1.0, 0.0);
    addPxyz(maxX, maxY, minZ); addNxyz(0.0, 1.0, 0.0);
    addPxyz(maxX, maxY, maxZ); addNxyz(0.0, 1.0, 0.0);
    addPxyz(minX, maxY, maxZ); addNxyz(0.0, 1.0, 0.0);
    addTriangleI012(1, 0, 2);
    addTriangleI012(3, 2, 0);

    // Bottom
    beginTriangles();
    addPxyz(minX, minY, minZ); addNxyz(0.0, -1.0, 0.0);
    addPxyz(maxX, minY, minZ); addNxyz(0.0, -1.0, 0.0);
    addPxyz(maxX, minY, maxZ); addNxyz(0.0, -1.0, 0.0);
    addPxyz(minX, minY, maxZ); addNxyz(0.0, -1.0, 0.0);
    addTriangleI012(0, 1, 2);
    addTriangleI012(2, 3, 0);

    // Back
    beginTriangles();
    addPxyz(minX, minY, minZ); addNxyz(0.0, 0.0, -1.0);
    addPxyz(maxX, minY, minZ); addNxyz(0.0, 0.0, -1.0);
    addPxyz(maxX, maxY, minZ); addNxyz(0.0, 0.0, -1.0);
    addPxyz(minX, maxY, minZ); addNxyz(0.0, 0.0, -1.0);
    addTriangleI012(1, 0, 2);
    addTriangleI012(3, 2, 0);

    // Front
    beginTriangles();
    addPxyz(minX, minY, maxZ); addNxyz(0.0, 0.0, 1.0);
    addPxyz(maxX, minY, maxZ); addNxyz(0.0, 0.0, 1.0);
    addPxyz(maxX, maxY, maxZ); addNxyz(0.0, 0.0, 1.0);
    addPxyz(minX, maxY, maxZ); addNxyz(0.0, 0.0, 1.0);
    addTriangleI012(0, 1, 2);
    addTriangleI012(2, 3, 0);

    return *this;
}

void MeshBuilder::beginTriangles()
{
    beginWithTopology(AGPU_TRIANGLES);
}

void MeshBuilder::beginWithTopology(agpu_primitive_topology topology)
{
    beginWithTopologyAndMaterial(topology, currentMaterial);
}

void MeshBuilder::beginWithTopologyAndMaterial(agpu_primitive_topology topology, const MaterialPtr &material)
{
    if(!primitives.empty())
    {
        auto &lastPrimitive = primitives.back();
        if(lastPrimitive.topology == topology && lastPrimitive.material == material)
        {
            baseVertex = positions.size();
            return;
        }
        finishLastPrimitive();
    }

    baseVertex = positions.size();
    MeshBuilderPrimitive primitive = {};
    primitive.material = currentMaterial;
    primitive.topology = topology;
    primitive.firstIndex = indices.size();

    primitives.push_back(primitive);
}

void MeshBuilder::finishLastPrimitive()
{
    if(primitives.empty())
        return;
    
    auto primitive = &primitives.back();
    primitive->indexCount = indices.size() - primitive->firstIndex;

}

void MeshBuilder::addPxyz(Scalar x, Scalar y, Scalar z)
{
    auto transformedPosition = currentTransform.transformPosition(Math::Vector3(x, y, z));
    positions.push_back(Woden::Math::CompactVector3(transformedPosition));
}

void MeshBuilder::addNxyz(Scalar x, Scalar y, Scalar z)
{
    normals.push_back(Woden::Math::CompactVector3(x, y, z));

}
void MeshBuilder::addTriangleI012(uint32_t i0, uint32_t i1, uint32_t i2)
{
    indices.push_back(i0 + baseVertex);
    indices.push_back(i1 + baseVertex);
    indices.push_back(i2 + baseVertex);
}

std::vector<MeshPrimitivePtr> MeshBuilder::encodePrimitives()
{
    std::vector<MeshPrimitivePtr> encodedPrimitives;
    encodedPrimitives.reserve(primitives.size());

    for(const auto &builderPrimitive : primitives)
    {
        auto indexAccessor = std::make_shared<Assets::BinaryBufferAccessor> ();
        indexAccessor->bufferView = indexBufferView;
        indexAccessor->byteOffset = builderPrimitive.firstIndex * indexBufferView->byteStride;
        indexAccessor->count = builderPrimitive.indexCount;
        indexAccessor->type = Assets::BinaryBufferAccessorType::Scalar;
        indexAccessor->componentType = indexBufferView->byteStride <= 2
            ? Assets::BinaryBufferAccessorComponentType::UInt16
            : Assets::BinaryBufferAccessorComponentType::UInt32;

        auto primitive = std::make_shared<MeshPrimitive> ();
        primitive->material = builderPrimitive.material;
        primitive->topology = builderPrimitive.topology;
        primitive->vertexBinding = vertexBinding;
        primitive->indices = indexAccessor;
        encodedPrimitives.push_back(primitive);
    }

    return encodedPrimitives;
}

MeshBuilder & MeshBuilder::generateTexcoordsWithFacePlanarTransformWithScale(const Math::Vector2 &scale)
{
    return generateTexcoordsWithFacePlanarTransform(Math::Matrix3x3::TextureScaleAndOffset(scale, Math::Vector2(0, 0)));
}

MeshBuilder & MeshBuilder::generateTexcoordsWithFacePlanarTransform(const Math::Matrix3x3 &transformMatrix)
{
    assert(positions.size() == normals.size());

    texcoords.resize(positions.size());
    for(size_t i = 0; i < positions.size(); ++i)
    {
        const auto &position = positions[i];
        const auto &normal = normals[i];

        auto normalAxis = Math::Vector3(normal.x, normal.y, normal.z).computeNormalAxis();
        auto tangent = Math::Vector3::TangentForAxis(normalAxis);
        auto bitangent = Math::Vector3::BitangentForAxis(normalAxis);

        auto positionVector = Math::Vector3(position.x, position.y, position.z);
        auto uv = Math::Vector2(tangent.dot(positionVector), bitangent.dot(positionVector));
        auto transformedUV = transformMatrix * uv;

        texcoords[i] = transformedUV;
    }
    return *this;
}

MeshBuilder &MeshBuilder::generateTangentSpaceFrame()
{
    assert(positions.size() == normals.size());
    assert(normals.size() == texcoords.size());
    finishLastPrimitive();

    std::vector<Math::Vector3> tangents;
    tangents.resize(positions.size(), Math::Vector3(0));

    std::vector<Math::Vector3> bitangents;
    bitangents.resize(positions.size(), Math::Vector3(0));

    for(auto &primitive : primitives)
    {
        if(primitive.topology == AGPU_TRIANGLES)
        {
            for(size_t i = 0; i < primitive.indexCount; i+=3)
            {
                auto i0 = indices[primitive.firstIndex + i + 0];
                auto i1 = indices[primitive.firstIndex + i + 1];
                auto i2 = indices[primitive.firstIndex + i + 2];

                auto p0 = positions[i0].asVector3();
                auto p1 = positions[i1].asVector3();
                auto p2 = positions[i2].asVector3();

                auto tc0 = texcoords[i0];
                auto tc1 = texcoords[i1];
                auto tc2 = texcoords[i2];

                auto u = p1 - p0;
                auto v = p2 - p0;

                auto du0 = tc1.x - tc0.x;
                auto dv0 = tc1.y - tc0.y;
                
                auto du1 = tc2.x - tc0.x;
                auto dv1 = tc2.y - tc0.y;
                
                auto det = (du0*dv1) - (du1*dv0);
                if(det != 0)
                {
                    auto tangent = (u * Math::Vector3(dv1/det))  - (v * Math::Vector3(dv0/det));
                    auto bitangent = (v * Math::Vector3(du0/det)) - (u * Math::Vector3(du1/det));

                    tangents[i0] += tangent;
                    tangents[i1] += tangent;
                    tangents[i2] += tangent;

                    bitangents[i0] += bitangent;
                    bitangents[i1] += bitangent;
                    bitangents[i2] += bitangent;
                }
            }
        }
    }

    tangents4.resize(positions.size());
    for(size_t pi = 0; pi < positions.size(); ++pi)
    {
        auto t = tangents[pi];
        auto b = bitangents[pi];
        auto n = normals[pi].asVector3();

        // Gram schmidth orthogonalization
        t = (t - (n * Math::Vector3(n.dot(t)))).normalized();
        b = (b - (n * Math::Vector3(n.dot(b))) - (t * Math::Vector3(t.dot(b)))).normalized();
        auto s = n.cross(t).dot(b);
        
        //printf("pi %d | t: %f %f %f s: %f\n", (int)pi, t.x, t.y, t.z, s);

        tangents4[pi] = Math::Vector4(t.x, t.y, t.z, s);
    }

    return *this;
}

void MeshBuilder::encodeBufferData()
{
    buffer = std::make_shared<Assets::BinaryBuffer> ();

    // Vertex buffer
    vertexBufferView = std::make_shared<Assets::BinaryBufferView> ();
    vertexBufferView->buffer = buffer;
    vertexBufferView->byteOffset = buffer->data.size();

    vertexBinding = std::make_shared<VertexBinding> ();

    // Position
    {
        auto startOffset = buffer->data.size();
        buffer->addDataFromVector(positions);
        vertexBinding->addAttributeWithBufferView(
            VertexBufferAttribute::PositionLocation, vertexBufferView,
            startOffset, positions.size(),
            Assets::BinaryBufferAccessorType::Vector3, Assets::BinaryBufferAccessorComponentType::Float32
        );
    }

    // Normal
    {
        normals.resize(positions.size());

        auto startOffset = buffer->data.size();
        buffer->addDataFromVector(normals);
        vertexBinding->addAttributeWithBufferView(
            VertexBufferAttribute::NormalLocation, vertexBufferView,
            startOffset, normals.size(),
            Assets::BinaryBufferAccessorType::Vector3, Assets::BinaryBufferAccessorComponentType::Float32
        );
    }

    // Texcoords
    {
        if(texcoords.size() != positions.size())
            texcoords.resize(positions.size());

        auto startOffset = buffer->data.size();
        buffer->addDataFromVector(texcoords);
        vertexBinding->addAttributeWithBufferView(
            VertexBufferAttribute::Texcoord0Location, vertexBufferView,
            startOffset, texcoords.size(),
            Assets::BinaryBufferAccessorType::Vector2, Assets::BinaryBufferAccessorComponentType::Float32
        );
    }

    // Tangent4
    {
        if(tangents4.empty() && !positions.empty())
            generateTangentSpaceFrame();

        auto startOffset = buffer->data.size();
        buffer->addDataFromVector(tangents4);
        vertexBinding->addAttributeWithBufferView(
            VertexBufferAttribute::Tangent4Location, vertexBufferView,
            startOffset, tangents4.size(),
            Assets::BinaryBufferAccessorType::Vector4, Assets::BinaryBufferAccessorComponentType::Float32
        );
    }

    vertexBufferView->byteLength = buffer->data.size() - vertexBufferView->byteOffset;

    // Index buffer.
    indexBufferView = std::make_shared<Assets::BinaryBufferView> ();
    indexBufferView->buffer = buffer;
    indexBufferView->byteOffset = buffer->data.size();
    indexBufferView->byteStride = 4;

    buffer->addDataFromVector(indices);

    indexBufferView->byteLength = buffer->data.size() - indexBufferView->byteOffset;
}

RenderablePtr MeshBuilder::finishMesh()
{
    if(positions.empty())
        return std::make_shared<StaticMeshRenderable> ();
    finishLastPrimitive();
    encodeBufferData();

    auto mesh = std::make_shared<StaticMeshRenderable> ();
    mesh->primitives = encodePrimitives();
    return mesh;
}

} // End of namespace Rendering
} // End of namespace Woden