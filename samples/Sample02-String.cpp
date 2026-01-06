#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/StringMorph.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto morph = std::make_shared<StringMorph> ();
    morph->label = "Hello World";
    morph->setExtent(Vector2(128, 50));
    morph->openInSystemWindow();
    return 0;
}