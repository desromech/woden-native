#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
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
        auto material = std::make_shared<Woden::Rendering::MetallicRoughnessMaterial> ();
        material->roughnessFactor = 0.4;
        material->metallicFactor = 0.0;

        auto builder = Woden::Rendering::MeshBuilder();
        builder.currentMaterial = material;
        builder.addCubeWithExtent(Vector3(1, 1, 1));
        builder.generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1));
        scene->normalLayer->addChild(builder.finishMesh()->asSceneNode());
    }

    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8, 0.8, 0.2);
        pointLightSource->intensity = 5;
        pointLightSource->influenceRadius = 4;

        scene->normalLayer->addChild(pointLightSource->asSceneNodeWithPosition(Vector3(-1.5, 1.5, 1.6)));

    }

    scene->openInSystemWindow();

    int exitCode = Morph::runMainLoop();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}