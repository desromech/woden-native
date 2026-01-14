#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/Morph.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto morph = std::make_shared<Morph> ();
    morph->setExtent(Vector2(1024, 576));
    morph->openInSystemWindow();

    return 0;
}