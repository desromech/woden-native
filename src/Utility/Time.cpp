#include "Woden/Utility/Time.hpp"
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#else
#   include <time.h>
#endif
namespace Woden
{
namespace Utility
{

#ifndef _WIN32
double getCurrentMilliseconds()
{
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);

    return double(tp.tv_sec)*1000 + double(tp.tv_nsec)/1000000;
}
#else
double getCurrentMilliseconds()
{
    LARGE_INTEGER ticks;
    LARGE_INTEGER tickFrequency;
    if (!QueryPerformanceCounter(&ticks) || !QueryPerformanceFrequency(&tickFrequency))
        abort();
    
    auto microseconds = ticks.QuadPart * 1000000 / tickFrequency.QuadPart;
    return microseconds*0.001;
}
#endif

} // End of namespace Utility
} // End of namespace Woden