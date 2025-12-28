#ifndef WODEN_ASSET_BINARY_BUFFER_HPP
#define WODEN_ASSET_BINARY_BUFFER_HPP

#include <stdint.h>
#include <memory>
#include <string>
#include <vector>

namespace Woden
{
namespace Assets
{
typedef std::shared_ptr<class BinaryBuffer> BinaryBufferPtr;
typedef std::shared_ptr<class BinaryBufferView> BinaryBufferViewPtr;

class BinaryBuffer
{
public:
    std::string name;
    std::vector<uint8_t> data;

    template <typename D>
    void addDataFromVector(const std::vector<D> &inputVector)
    {
        auto start = reinterpret_cast<const uint8_t*> (inputVector.data());
        auto end = reinterpret_cast<const uint8_t*> (inputVector.data() + inputVector.size());
        data.insert(data.end(), start, end);
    }
};

class BinaryBufferView
{
public:
    std::string name;
	size_t byteOffset = 0;
	size_t byteStride = 0;
	size_t byteLength = 0;
    BinaryBufferPtr buffer;
};

} // End of namespace Assets
} // End of namespace Woden

#endif //WODEN_ASSET_BINARY_BUFFER_HPP
