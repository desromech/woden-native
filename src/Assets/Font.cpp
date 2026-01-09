#include "Woden/Assets/Font.hpp"
#include "Woden/Assets/Image.hpp"
#include "Woden/Utility/ReadWholeFile.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "Woden/Assets/stb_truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION

namespace Woden
{
namespace Assets
{

FontPtr Font::loadFromFile(const std::string &filename)
{
    auto data = Utility::readWholeFile(filename);
    if(data.empty())
        return nullptr;

    auto font = std::make_shared<Font> ();
    font->rawData.swap(data);

    return font;
}

FontFacePtr Font::getOrCreateFaceWithHeight(uint32_t height)
{
    // Create the image.
    auto image = std::make_shared<Image> ();
    image->width = height < 20 ? 256 : 512;
    image->height = height < 30 ? 256 : 512;
    image->pitch = image->width;
    image->format = PixelFormat::R8_Unorm;
    image->pixels.resize(image->pitch*image->height);

    // Create the font face.
    auto face = std::make_shared<FontFace> ();
    face->image = image;
    face->bakedChars.resize(256-31);

    stbtt_BakeFontBitmap(rawData.data(), 0, float(height), image->pixels.data(), image->width, image->height, 31, 256 - 31, face->bakedChars.data());
    stbtt_GetScaledFontVMetrics(rawData.data(), 0, float(height), &face->ascent, &face->descent, &face->linegap);
    
    return face;
}

}// End of namespace Assets
}// End of namespace Woden