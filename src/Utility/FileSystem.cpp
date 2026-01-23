#include "Woden/Utility/FileSystem.hpp"
#include <algorithm>
#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#endif

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

std::string getCurrentWorkingDirectory()
{
#ifdef _WIN32
    return "."
#else
    char *buffer = getcwd(nullptr, 0);
    std::string result = buffer;
    free(buffer);
    return result;
#endif
}

std::vector<DirectoryEntry> readWholeDirectory(const std::string &directoryPath)
{
    std::vector<DirectoryEntry> result;
#ifdef _WIN32
#else
    auto dir = opendir(directoryPath.c_str());
    if(!dir)
        return result;

    dirent *entry = NULL;
    while((entry = readdir(dir)) != NULL)
    {
        DirectoryEntry dirEntry;
        dirEntry.name = entry->d_name;
        dirEntry.isDirectory = entry->d_type == DT_DIR;
        if(dirEntry.name == "." || dirEntry.name == "..")
            continue;;

        result.push_back(dirEntry);
    }

    closedir(dir);
#endif
    return result;
}

std::vector<DirectoryEntry> readWholeDirectorySorted(const std::string &directoryPath)
{
    auto entries = readWholeDirectory(directoryPath);
    std::sort(entries.begin(), entries.end(), [](const DirectoryEntry &a, DirectoryEntry &b){
        if(a.isDirectory != b.isDirectory)
            return a.isDirectory > b.isDirectory;
        return a.name < b.name;
    });
    return entries;
}
} // End of namespace Utility
} // End of namespace Woden