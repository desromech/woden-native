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
        scene->normalLayer->addChild(Woden::Rendering::MeshBuilder()
            .setMaterial(Woden::Assets::ResourceCache::Get()->getOrCreateCheckboardMaterial())
		    .translateTo(Vector3(0, -0.125, 0))
		    .addCubeWithExtent(Vector3(10.0, 0.25, 10.0))

		    .translateTo(Vector3(0, 0.5, 0))
		    .addCubeWithExtent(Vector3(1.0, 1.0, 1.0))

		    .translateTo(Vector3(2, 0.25, 2))
		    .addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

		    .translateTo(Vector3(1, 0.25, 2))
		    .addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

		    .translateTo(Vector3(0, 0.25, 2))
		    .addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

		    .translateTo(Vector3(-1, 0.25, 2))
		    .addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

		    .translateTo(Vector3(-2, 0.25, 2))
		    .addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

            .generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1))
            .generateTangentSpaceFrame()
            .finishMesh()->asSceneNode()
        );
    }

    {
        auto spotLightSource = std::make_shared<Woden::Rendering::SpotLightSource> ();
        spotLightSource->color = Vector3(0.8f, 0.8f, 0.7f);
        spotLightSource->intensity = 20;
        spotLightSource->influenceRadius = 10;
        spotLightSource->castShadows = true;

        auto spotLightNode = spotLightSource->asSceneNode();
        spotLightNode->transform.rotation = Quaternion::YRotationDegrees(-135)*Quaternion::XRotationDegrees(-45);
        spotLightNode->transform.translation = Vector3(-1.0, 2.0, -2.0);

        scene->normalLayer->addChild(spotLightNode);
    }

    scene->openInSystemWindow();
    return 0;
}