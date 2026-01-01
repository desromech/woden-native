#include "Woden/Math/Matrix3x3.hpp"

namespace Woden
{
namespace Math
{
const Matrix3x3 Matrix3x3::CubeMapFaceRotations[6] = {
    // +X
    Matrix3x3(
        0.0, 0.0, -1.0,
        0.0, 1.0,  0.0,
        1.0, 0.0,  0.0
    ),
    // -X
    Matrix3x3(
        0.0, 0.0,  1.0,
        0.0, 1.0,  0.0,
        -1.0, 0.0, 0.0
    ),
    // +Y
    Matrix3x3(
        1.0, 0.0, 0.0,
        0.0, 0.0, -1.0,
        0.0, 1.0, 0.0
    ),
    // -Y
    Matrix3x3(
        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, -1.0, 0.0
    ),
    // +Z
    Matrix3x3(
        -1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, -1.0
    ),
    // -Z
    Matrix3x3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    )
};

} // End of namespace Math
} // End of namespace Woden