#include "Woden/Math/Vector3.hpp"
#include <assert.h>

namespace Woden
{
namespace Math
{

const Vector3 Vector3::NormalAxis[12] = {
    Vector3( 1,  0,  0),
    Vector3(-1,  0,  0),
    Vector3( 0,  1,  0),
    Vector3( 0, -1,  0),
    Vector3( 0,  0,  1),
    Vector3( 0,  0, -1),

    Vector3( 1,  0,  0),
    Vector3(-1,  0,  0),
    Vector3( 0,  1,  0),
    Vector3( 0, -1,  0),
    Vector3( 0,  0,  1),
    Vector3( 0,  0, -1)
};

uint32_t Vector3::computeNormalAxis()
{
    Scalar bestAmount = -INFINITY;
    int best = -1;

    for(int i = 0; i < 6; ++i)
    {
        auto amount = dot(NormalAxis[i]);
        if(amount > bestAmount)
        {
            bestAmount = amount;
            best = i;
        }
    }

    assert(best >= 0 && "Invalid normal");
    return best;
}
}
}