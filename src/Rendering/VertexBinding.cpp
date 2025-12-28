#include "Woden/Rendering/VertexBinding.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"

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

void VertexBinding::useWithRenderer(SceneRenderer *sceneRenderer)
{
    auto context = RenderingContext::getMainContext();
    if(!handle)
    {
        handle = context->device->createVertexBinding(context->staticVertexLayout);

        std::vector<agpu_buffer_ref> buffers;
        std::vector<agpu_size> offsets;
        buffers.reserve(attributes.size());
        offsets.reserve(offsets.size());

        for(auto &attr : attributes)
        {
            buffers.push_back(attr.accessor->bufferView->buffer->getOrCreateBufferWithContents());
            offsets.push_back(attr.accessor->byteOffset + attr.accessor->bufferView->byteOffset);
        }

        handle->bindVertexBuffersWithOffsets(buffers.size(), buffers.data(), offsets.data());
    }

    sceneRenderer->currentCommandList->useVertexBinding(handle);
}

} // End of namespace Rendering
} // End of namespace Woden