#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/Morph.hpp"
#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::Morphic;
using namespace Woden::SceneGraph;

int woden_main(int argc, const char **argv)
{
    auto scene = MakeScene();
    scene->openInSystemWindow();
    return 0;
}