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
typedef std::shared_ptr<class BinaryBufferAccessor> BinaryBufferAccessorPtr;

/**
 * A binary buffer.
 */
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

/**
 * A binary buffer view.
 */
class BinaryBufferView
{
public:
    std::string name;
	size_t byteOffset = 0;
	size_t byteStride = 0;
	size_t byteLength = 0;
    BinaryBufferPtr buffer;
};

/**
 * Binary buffer type.
 */
enum class BinaryBufferAccessorType
{
    Scalar,
    Vector2,
    Vector3,
    Vector4,
    Matrix4x4
};

/**
 * Binary buffer component type.
 */
enum class BinaryBufferAccessorComponentType
{
    Float32,
    Float64,
    Int16,
    Int32,
    Int64,
    UInt16,
    UInt32,
    UInt64,
};

/**
 * A binary buffer accessor.
 */
class BinaryBufferAccessor
{
public:
    std::string name;
    size_t byteOffset = 0;
    size_t count = 0;
    bool normalized = false;
    BinaryBufferAccessorType type = BinaryBufferAccessorType::Scalar;
    BinaryBufferAccessorComponentType componentType = BinaryBufferAccessorComponentType::Float32;
	BinaryBufferViewPtr bufferView;
};

} // End of namespace Assets
} // End of namespace Woden

#endif //WODEN_ASSET_BINARY_BUFFER_HPP
