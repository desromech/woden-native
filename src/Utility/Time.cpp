#include "Woden/Utility/Time.hpp"
#include <time.h>

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
#error TODO: implement getCurrentMilliseconds for windows.
#endif

} // End of namespace Utility
} // End of namespace Woden