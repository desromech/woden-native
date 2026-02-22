#include "Assets/QMapFile.hpp"
#include "Utility/ReadWholeFile.hpp"

namespace Woden
{
namespace Assets
{
    
QMapFilePtr QMapFile::parseFromFileNamed(const std::string &filename)
{
    auto string = Utility::readWholeTextFile(filename);
    if(string.empty())
        return nullptr;
    return parseFromString(string);
}

QMapFilePtr QMapFile::parseFromString(const std::string &string)
{
    auto map = std::make_shared<QMapFile> ();
    return map;
}

} // End of namespace Assets
} // namespace Woden
