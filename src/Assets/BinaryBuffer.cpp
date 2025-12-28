#include "Woden/Assets/BinaryBuffer.hpp"
#include "Woden/Rendering/Context.hpp"

namespace Woden
{
namespace Assets
{

agpu_buffer_ref BinaryBuffer::getOrCreateBufferWithContents()
{
    if(handle)
        return handle;

    agpu_buffer_description desc = {};
    desc.size = data.size();
    desc.heap_type = AGPU_MEMORY_HEAP_TYPE_DEVICE_LOCAL;
    desc.usage_modes = desc.main_usage_mode = agpu_buffer_usage_mask(AGPU_ARRAY_BUFFER | AGPU_ELEMENT_ARRAY_BUFFER);
    desc.mapping_flags = AGPU_MAP_DYNAMIC_STORAGE_BIT;
    desc.stride = 0;

    handle = Rendering::RenderingContext::getMainContext()->device->createBuffer(&desc, data.data());
    if(!handle)
    {
        fprintf(stderr, "Failed to create vertex/index buffer.");
        abort();
    }
    return handle;
}

} // End of namespace Assets
} // End of namespace Woden