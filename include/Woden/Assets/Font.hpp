#ifndef WODEN_ASSETS_FONT_HPP
#define WODEN_ASSETS_FONT_HPP

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
#include "stb_truetype.h"

namespace Woden
{
namespace Assets
{

typedef std::shared_ptr<class Font> FontPtr;
typedef std::shared_ptr<class FontFace> FontFacePtr;
typedef std::shared_ptr<class Image> ImagePtr;

class Font
{
public:
    static FontPtr loadFromFile(const std::string &filename);

    FontFacePtr getOrCreateFaceWithHeight(uint32_t height);

    std::vector<uint8_t> rawData;
};

class FontFace
{
public:
    float ascent = 0;
    float descent = 0;
    float linegap = 0;

    ImagePtr image;
    std::vector<stbtt_bakedchar> bakedChars;
};

}// End of namespace Assets
}// End of namespace Woden

#endif //WODEN_ASSETS_FONT_HPP