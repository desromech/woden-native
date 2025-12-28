#ifndef WODEN_RENDERING_VERTEX_BINDING_HPP
#define WODEN_RENDERING_VERTEX_BINDING_HPP

#include "AGPU/agpu.hpp"
#include "Woden/Assets/BinaryBuffer.hpp"
#include <memory>
#include <vector>

namespace Woden
{
namespace Rendering
{
typedef std::shared_ptr<class VertexBinding> VertexBindingPtr;

class VertexBufferAttribute
{
public:
	static const uint32_t PositionLocation    = 0;
	static const uint32_t NormalLocation      = 1;
	static const uint32_t Texcoord0Location   = 2;
	static const uint32_t Texcoord1Location   = 3;
	static const uint32_t BoneIndicesLocation = 4;
	static const uint32_t BoneWeightsLocation = 5;
	static const uint32_t Tangent4Location    = 6;

    uint32_t location = 0;
    Assets::BinaryBufferAccessorPtr accessor;
};

class VertexBinding
{
public:
    void addAttribute(uint32_t location, Assets::BinaryBufferAccessorPtr accessor);
    void addAttributeWithBufferView(uint32_t location, Assets::BinaryBufferViewPtr bufferView, size_t byteOffset, size_t count, Assets::BinaryBufferAccessorType accessorType, Assets::BinaryBufferAccessorComponentType accessorComponentType);

    uint32_t vertexCount = 0;
    std::vector<VertexBufferAttribute> attributes;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif // WODEN_RENDERING_VERTEX_BINDING_HPP