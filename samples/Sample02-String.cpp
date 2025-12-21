#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/StringMorph.hpp"

using namespace Woden::Morphic;

int main(int argc, const char *argv[])
{
    if(!Woden::Rendering::RenderingContext::getOrCreateMainContext(argc, argv))
        return 1;

    auto morph = std::make_shared<StringMorph> ();
    morph->label = "Hello World";
    morph->setExtent(Vector2(128, 50));
    morph->openInSystemWindow();

    int exitCode = Morph::runMainLoop();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}