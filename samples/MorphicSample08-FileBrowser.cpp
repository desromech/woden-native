#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/FileBrowser.hpp"
#include "Woden/Utility/FileSystem.hpp"

using namespace Woden::Morphic;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;

    auto browser = MakeMorph<OpenFileBrowser> ();
    browser->setDirectory(Woden::Utility::getCurrentWorkingDirectory());
    browser->onAcceptedFile = [&](std::string path) {
        printf("Accepted file: %s\n", path.c_str());
    };
    browser->openInSystemWindow();

    return 0;
}
