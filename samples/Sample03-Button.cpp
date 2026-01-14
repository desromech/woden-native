#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/SimpleButtonMorph.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto morph = std::make_shared<SimpleButtonMorph> ();
    morph->setLabel("Click me");
    morph->setExtent(Vector2(128, 50));
    morph->openInSystemWindow();
    return 0;
}
