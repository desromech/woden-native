#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::SceneGraph;
using namespace Woden::Math;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto scene = MakeScene();
    {
        auto material = std::make_shared<Woden::Rendering::MetallicRoughnessMaterial> ();
        material->roughnessFactor = 0.4;
        material->metallicFactor = 0.0;
        material->baseColorTexture = Woden::Assets::ResourceCache::Get()->getOrCreateCheckboardTexture();
        material->normalTexture = Woden::Assets::ResourceCache::Get()->getOrCreateCheckboardNormalTexture();

        scene->normalLayer->addChild(Woden::Rendering::MeshBuilder()
            .setMaterial(material)
            .addCubeWithExtent(Vector3(1, 1, 1))
            .generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1))
            .finishMesh()->asSceneNode()
        );
    }

    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8, 0.8, 0.2);
        pointLightSource->intensity = 5;
        pointLightSource->influenceRadius = 4;

        scene->normalLayer->addChild(pointLightSource->asSceneNodeWithPosition(Vector3(-1.5, 1.5, 1.6)));
    }

    scene->openInSystemWindow();
    return 0;
}