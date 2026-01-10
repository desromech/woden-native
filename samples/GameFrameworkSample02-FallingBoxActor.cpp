#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"
#include "Woden/GameFramework/World.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::GameFramework;
using namespace Woden::SceneGraph;
using namespace Woden::Math;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;

    auto world = std::make_shared<World> ();

    // Cube actor
    {
        auto cubeMesh = Woden::Rendering::MeshBuilder()
            .addCubeWithExtent(Vector3(1, 1, 1))
            .generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1))
            .finishMesh();
        
        auto actor = std::make_shared<Actor> ();
        auto meshComponent = std::make_shared<ActorMeshSceneComponent> ();
        meshComponent->mesh = cubeMesh;
        actor->addComponent(meshComponent);
        world->spawnActor(actor);
    }

    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8f, 0.8f, 0.2f);
        pointLightSource->intensity = 5;
        pointLightSource->influenceRadius = 4;

        auto lightComponent = std::make_shared<ActorLightSourceComponent> ();
        lightComponent->light = pointLightSource;

        auto lightSourceActor = std::make_shared<Actor> ();
        lightSourceActor->addComponent(lightComponent);
        lightSourceActor->setPosition(Vector3(0, 1.5, 2));
        world->spawnActor(lightSourceActor);
    }
    
    world->playInSystemWindow();
    return 0;
}