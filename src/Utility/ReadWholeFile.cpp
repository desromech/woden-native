#include "Woden/Utility/ReadWholeFile.hpp"
#include <vector>

namespace Woden
{
namespace Utility
{

std::vector<uint8_t> readWholeFile(const std::string &fileName)
{
    FILE *file = fopen(fileName.c_str(), "rb");
    if(!file)
    {
        fprintf(stderr, "Failed to open file %s\n", fileName.c_str());
        return std::vector<uint8_t>();
    }

    // Allocate the data.
    std::vector<uint8_t> data;
    fseek(file, 0, SEEK_END);
    data.resize(ftell(file));
    fseek(file, 0, SEEK_SET);

    // Read the file
    if(fread(&data[0], data.size(), 1, file) != 1)
    {
        fprintf(stderr, "Failed to read file %s\n", fileName.c_str());
        fclose(file);
        return std::vector<uint8_t>();
    }

    fclose(file);
    return data;
}


std::string readWholeTextFile(const std::string &fileName)
{
    FILE *file = fopen(fileName.c_str(), "rb");
    if(!file)
    {
        fprintf(stderr, "Failed to open file %s\n", fileName.c_str());
        return std::string();
    }

    // Allocate the data.
    std::vector<char> data;
    fseek(file, 0, SEEK_END);
    data.resize(ftell(file));
    fseek(file, 0, SEEK_SET);

    // Read the file
    if(fread(&data[0], data.size(), 1, file) != 1)
    {
        fprintf(stderr, "Failed to read file %s\n", fileName.c_str());
        fclose(file);
        return std::string();
    }

    fclose(file);
    return std::string(data.begin(), data.end());
}

} // End of namespace Utility
} // End of namespace Woden
