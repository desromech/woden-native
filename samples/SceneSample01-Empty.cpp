#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::SceneGraph;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto scene = MakeScene();
    scene->openInSystemWindow();
    return 0;
}