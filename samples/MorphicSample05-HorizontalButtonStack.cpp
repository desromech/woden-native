#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/SimpleButtonMorph.hpp"
#include "Woden/Morphic/Layout.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto morph = std::make_shared<Morph> ();
    auto layout = std::make_shared<HorizontalPackingLayout> ();

    for(int i = 0; i < 5; ++i)
    {
        char label[64];
        snprintf(label, sizeof(label), "Click Me %d", i);
        auto button = std::make_shared<SimpleButtonMorph> ();
        button->setLabel(label);
        button->onClickAction = [=](const MorphPtr &morph){
            (void)morph;
            printf("Clicked button %d\n", i);
        };

        morph->addMorph(button);
        layout->addMorph(button, 0);
    }

    morph->setLayout(layout);
    morph->setExtent(Vector2(500, 50));
    morph->openInSystemWindow();
    return 0;
}
