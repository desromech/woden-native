#include "Woden/Assets/Image.hpp"
#include <stdio.h>

namespace Woden
{
namespace Assets
{

struct __attribute__((packed)) TGAHeader
{
    uint8_t idLength;
    uint8_t colorMapType;
    uint8_t imageType;

    uint16_t colorMapFirstEntry;
    uint16_t colorMapLengh;
    uint8_t colorMapEntrySize;

    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t pixelDepth;
    uint8_t imageDescriptor;
};

bool Image::saveToTGA(const std::string &filename)
{
    FILE *file = fopen(filename.c_str(), "wb");
    if(!file)
        return false;

    TGAHeader header = {0};
    header.imageType = 2;
    header.width = width;
    header.height = height;
    header.pixelDepth = 32;

    if(1 != fwrite(&header, sizeof(TGAHeader), 1, file))
    {
        perror("Failed to write TGA header");
        fclose(file);
        return false;
    }

    if(1 != fwrite(pixels.data(), pixels.size(), 1, file))
    {
        perror("Failed to write TGA pixels");
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}


}// End of namespace Assets
}// End of namespace Woden
