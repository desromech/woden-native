#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/Morph.hpp"

using namespace Woden::Morphic;

int main(int argc, const char *argv[])
{
    if(!Woden::Rendering::RenderingContext::getOrCreateMainContext(argc, argv))
        return 1;

    auto morph = std::make_shared<Morph> ();
    morph->setExtent(Vector2(1024, 576));
    morph->openInSystemWindow();

    int exitCode = Morph::runMainLoop();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}