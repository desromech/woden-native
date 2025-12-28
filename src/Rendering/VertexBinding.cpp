#include "Woden/Rendering/VertexBinding.hpp"
namespace Woden
{
namespace Rendering
{
void VertexBinding::addAttribute(uint32_t location, Assets::BinaryBufferAccessorPtr accessor)
{
    VertexBufferAttribute attribute;
    attribute.location = location;
    attribute.accessor = accessor;
    attributes.push_back(attribute);
}

void VertexBinding::addAttributeWithBufferView(uint32_t location, Assets::BinaryBufferViewPtr bufferView, size_t byteOffset, size_t count, Assets::BinaryBufferAccessorType accessorType, Assets::BinaryBufferAccessorComponentType accessorComponentType)
{
    auto accessor = std::make_shared<Assets::BinaryBufferAccessor> ();
    accessor->bufferView = bufferView;
    accessor->byteOffset = byteOffset;
    accessor->count = count;
    accessor->type = accessorType;
    accessor->componentType = accessorComponentType;
    addAttribute(location, accessor);
}

} // End of namespace Rendering
} // End of namespace Woden