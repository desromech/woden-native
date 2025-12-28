#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/Morphic/Morph.hpp"
#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::Morphic;
using namespace Woden::SceneGraph;
using namespace Woden::Math;

int main(int argc, const char *argv[])
{
    if(!Woden::Rendering::RenderingContext::getOrCreateMainContext(argc, argv))
        return 1;

    auto scene = MakeScene();
    {
        auto builder = Woden::Rendering::MeshBuilder();
        builder.addCubeWithExtent(Vector3(1, 1, 1));
        scene->normalLayer->addChild(builder.finishMesh()->asSceneNode());
    }

    scene->openInSystemWindow();

    int exitCode = Morph::runMainLoop();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}