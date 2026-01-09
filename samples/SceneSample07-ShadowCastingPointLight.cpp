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

			// Floor
			.translateTo(Vector3(0, -0.125, 0))
            .addCubeWithExtent(Vector3(10.0, 0.25, 10.0))

            // Ceiling
			.translateTo(Vector3(0, 3, 0))
            .addCubeWithExtent(Vector3(10.0, 0.25, 10.0))

            // Front Wall
			.translateTo(Vector3(0, 1.5, -5.0))
			.addCubeWithExtent(Vector3(10.0, 3.0, 0.25))

			// Back Wall
			.translateTo(Vector3(0, 1.5, 5.0))
			.addCubeWithExtent(Vector3(10.0, 3.0, 0.25))

			// Left Wall
			.translateTo(Vector3(-5.0, 1.5, 0.0))
			.addCubeWithExtent(Vector3(0.25, 3.0, 10.0))

			// Right Wall
			.translateTo(Vector3(5.0, 1.5, 0.0))
			.addCubeWithExtent(Vector3(0.25, 3.0, 10.0))

            // Center cube
            .translateTo(Vector3(0, 0.5, 0))
			.addCubeWithExtent(Vector3(1.0, 1.0, 1.0))

            // Cube row
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

            // Top row
            .translateTo(Vector3(2, 2.25, -1))
			.addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

            .translateTo(Vector3(1, 2.25, -1))
			.addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

            .translateTo(Vector3(0, 2.25, -1))
			.addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

            .translateTo(Vector3(-1, 2.25, -1))
			.addCubeWithExtent(Vector3(0.5, 0.5, 0.5))

            .translateTo(Vector3(-2, 2.25, -1))
			.addCubeWithExtent(Vector3(0.5, 0.5, 0.5))
            
            .generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1))
            .generateTangentSpaceFrame()
            .finishMesh()->asSceneNode()
        );
    }

    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8f, 0.8f, 0.7f);
        pointLightSource->intensity = 10;
        pointLightSource->influenceRadius = 10;
        pointLightSource->castShadows = true;

        scene->normalLayer->addChild(pointLightSource->asSceneNodeWithPosition(Vector3(0, 1.5, 0)));
    }

    scene->openInSystemWindow();
    return 0;
}