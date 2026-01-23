#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/SimpleButtonMorph.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto morph = MakeMorph<SimpleButtonMorph> ();
    morph->setLabel("Click me");
    morph->onClickAction = [](const MorphPtr &morph){
        (void)morph;
        printf("Clicked button\n");
    };

    morph->setExtent(Vector2(128, 50));
    morph->openInSystemWindow();
    return 0;
}
