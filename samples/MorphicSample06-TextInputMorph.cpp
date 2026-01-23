#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/TextInputMorph.hpp"
#include "Woden/Morphic/Layout.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto textInput = MakeMorph<TextInputMorph> ();
    textInput->openInSystemWindow();
    return 0;
}
