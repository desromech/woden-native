#ifndef WODEN_MATH_SCALAR_HPP
#define WODEN_MATH_SCALAR_HPP

#include <math.h>

namespace Woden
{
namespace Math
{

typedef float Scalar;

const Scalar CloseToEpsilon = 1.0e-6;

inline Scalar abs(Scalar x)
{
    return x <= 0 ? -x : x;
}

template<typename T>
T mix(const T&a, const T&b, Scalar alpha)
{
    return a*(1 - alpha) + b*alpha;
}

inline bool closeTo(Scalar a, Scalar b)
{
    float delta = a - b;
    return -CloseToEpsilon <= delta && delta <= CloseToEpsilon;
}

inline Scalar sign(Scalar v)
{
    if (v < 0)
        return -1;
    else if (v > 0)
        return 1;
    return 0;
}

inline Scalar sqrt(Scalar angle)
{
    return ::sqrt(angle);
}

inline Scalar cos(Scalar angle)
{
    return ::cos(angle);
}

inline Scalar sin(Scalar angle)
{
    return ::sin(angle);
}

}    
}

#endif // WODEN_MATH_SCALAR_HPP
