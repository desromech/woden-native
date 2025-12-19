#include "Woden/Assets/Image.hpp"

int main()
{
    auto image = std::make_shared<Woden::Assets::Image> ();
    image->width = 640;
    image->height = 480;
    image->pitch = image->width*4;
    image->format = Woden::Assets::PixelFormat::B8G8R8A8_UNorm;
    image->pixels.resize(image->pitch*image->height);

    image->saveToTGA("test.tga");
    return 0;
}