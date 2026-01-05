#ifndef _WODEN_UTILITY_FILE_SYSTEM_HPP
#define _WODEN_UTILITY_FILE_SYSTEM_HPP

#include <string>

namespace Woden
{
namespace Utility
{

std::string dirname(const std::string &path);
std::string basename(const std::string &path);
std::string extension(const std::string &path);

bool isAbsolutePath(const std::string &path);
std::string join(const std::string &leftPath, const std::string &rightPath);


} // End of namespace Utility
} // End of namespace Woden

#endif //_WODEN_UTILITY_FILE_SYSTEM_HPP