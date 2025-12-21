#ifndef _WODEN_UTILITY_READ_WHOLE_FILE_HPP
#define _WODEN_UTILITY_READ_WHOLE_FILE_HPP

#include <stdint.h>
#include <string>
#include <vector>

namespace Woden
{
namespace Utility
{

std::vector<uint8_t> readWholeFile(const std::string &fileName);
std::string readWholeTextFile(const std::string &fileName);

} // End of namespace Utility
} // End of namespace Woden

#endif //_WODEN_UTILITY_READ_WHOLE_FILE_HPP