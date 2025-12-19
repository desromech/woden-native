#include "Woden/Assets/Image.hpp"

void renderRGTestImage()
{
    auto image = std::make_shared<Woden::Assets::Image> ();
    image->width = 640;
    image->height = 480;
    image->pitch = image->width*4;
    image->format = Woden::Assets::PixelFormat::B8G8R8A8_UNorm;
    image->pixels.resize(image->pitch*image->height);
    image->renderPixels32([](uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        return ((y & 255) << 8) | ((x & 255) << 16) | 0xff000000;
    });

    image->saveToTGA("rgTest.tga");
}

int main()
{
    renderRGTestImage();
    return 0;
}