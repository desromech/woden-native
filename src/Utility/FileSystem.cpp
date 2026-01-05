#include "Woden/Utility/FileSystem.hpp"

namespace Woden
{
namespace Utility
{

std::string dirname(const std::string &path)
{
    size_t lastDirSeparatorLocation = 0;
    for(size_t i = 0; i < path.size(); ++i)
    {
        auto c = path[i];
        if(c == '/' || c == '\\')
            lastDirSeparatorLocation = i;
    }

    return path.substr(0, lastDirSeparatorLocation);
}

std::string basename(const std::string &path)
{
    size_t lastDirSeparatorLocation = 0;
    for(size_t i = 0; i < path.size(); ++i)
    {
        auto c = path[i];
        if(c == '/' || c == '\\')
            lastDirSeparatorLocation = i;
    }

    return path.substr(lastDirSeparatorLocation + 1);
}

std::string extension(const std::string &path)
{
    size_t position = path.rfind('.');
    return path.substr(position);
}

bool isAbsolutePath(const std::string &path)
{
#ifdef _WIN32
    return path.size() >= 2 && path[1] == ':';
#else
    return path.size() >= 1 && path[0] == '/';
#endif

}
std::string join(const std::string &leftPath, const std::string &rightPath)
{
    if (leftPath.empty() || isAbsolutePath(rightPath))
        return rightPath;

    auto result = leftPath;
    if(leftPath.back() != '/' && leftPath.back() != '\\')
        result.push_back('/');

    result += rightPath;
    return result;
}


} // End of namespace Utility
} // End of namespace Woden