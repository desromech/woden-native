#include "Woden/Utility/FileSystem.hpp"
#include <algorithm>
#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX 
#   include <Windows.h>
#else
#   include <unistd.h>
#   include <sys/types.h>
#   include <dirent.h>
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

#ifdef _WIN32
static WCHAR *
utf8ToWideChar(const char *string)
{
    size_t stringLen = strlen(string);
    int wstringSize = MultiByteToWideChar(CP_UTF8, 0, string, (int)stringLen, NULL, 0);
    if(wstringSize <= 0)
        return NULL;

    WCHAR *wstring = (WCHAR *)calloc(wstringSize + 1, sizeof(WCHAR));
    wstringSize = MultiByteToWideChar(CP_UTF8, 0, string, (int)stringLen, wstring, wstringSize + 1);
    if(wstringSize <= 0)
    {
        free(wstring);
        return NULL;
    }

    return wstring;
}

static std::string wideCharToUTF8(const WCHAR *wstring)
{
    int characterCount = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, NULL, 0, NULL, NULL);
    char *string = (char*)calloc(characterCount + 1, 1);
    WideCharToMultiByte(CP_UTF8, 0, wstring, -1, string, characterCount, NULL, NULL);
    std::string stdString = string;
    free(string);
    return stdString;
}

static WCHAR *
prepareWildCardPath(const char *path)
{
    const char * suffix = "\\*";
    size_t pathLength = strlen(path);
    size_t suffixLength = strlen(suffix);

    char *pathWithWildCard = (char *)calloc(pathLength + suffixLength + 1, 1);
    memcpy(pathWithWildCard, path, pathLength);
    memcpy(pathWithWildCard + pathLength, suffix, suffixLength);

    WCHAR *wpath = utf8ToWideChar(pathWithWildCard);
    free(pathWithWildCard);
    return wpath;
}

#endif

std::string getCurrentWorkingDirectory()
{
#ifdef _WIN32
    auto requiredSize = GetCurrentDirectoryW(0, NULL);
    auto buffer = (LPWSTR)calloc(1, (requiredSize + 2)*2);
    GetCurrentDirectoryW(requiredSize, buffer);
    auto result = wideCharToUTF8(buffer);
    free(buffer);
    return result;
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
    WCHAR *wpath = prepareWildCardPath(directoryPath.c_str());

    WIN32_FIND_DATAW findData;
    HANDLE findHandle = FindFirstFileW(wpath, &findData);
    free(wpath);
    if(findHandle == INVALID_HANDLE_VALUE)
    {
        return result;
    }

    {
        DirectoryEntry firstEntry;
        firstEntry.isDirectory = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        firstEntry.name = wideCharToUTF8(findData.cFileName);
        if(firstEntry.name != "." && firstEntry.name != "..")
            result.push_back(firstEntry);
    }

    while(FindNextFileW(findHandle, &findData))
    {
        DirectoryEntry entry;
        entry.isDirectory = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
        entry.name = wideCharToUTF8(findData.cFileName);
        if(entry.name != ".")
            result.push_back(entry);
    }

    FindClose(findHandle);

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
        if(dirEntry.name == ".")
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