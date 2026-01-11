#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"
#include "Woden/GameFramework/CollisionObjectComponents.hpp"
#include "Woden/GameFramework/CollisionShapeComponents.hpp"
#include "Woden/GameFramework/World.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::GameFramework;
using namespace Woden::SceneGraph;
using namespace Woden::Math;

struct Wall
{
    AABox box;
    Woden::Rendering::MaterialPtr material;    
};

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;

    auto world = std::make_shared<World> ();

    Woden::Rendering::MaterialPtr floorMaterial = Woden::Assets::ResourceCache::Get()->getOrCreateCheckboardMaterial();
    Woden::Rendering::MaterialPtr ceilingMaterial;
    Woden::Rendering::MaterialPtr wallMaterial;

    Wall walls[] = {
        {AABox(Vector3(-10, -0.5, -10), Vector3(10, 0.0, 10)), floorMaterial},
		{AABox(Vector3(-10,  5.0, -10), Vector3(10, 5.0, 10)), ceilingMaterial},

		{AABox(Vector3(-10, 0,  10), Vector3(10, 5.0, 10)), wallMaterial},
		{AABox(Vector3(-10, 0, -10), Vector3(10, 5.0, -10)), wallMaterial},

		{AABox(Vector3(-10, 0, -10), Vector3(-10, 5.0, 10)), wallMaterial},
		{AABox(Vector3( 10, 0, -10), Vector3( 10, 5.0, 10)), wallMaterial},
        
		{AABox(Vector3(-1, 0, -6), Vector3(1, 1.0, -5)), wallMaterial},
    };

    for(auto &wall : walls)
    {
        auto halfExtent = wall.box.halfExtent();
        auto center = wall.box.center();

        auto wallMesh = Woden::Rendering::MeshBuilder()
            .setMaterial(wall.material)
            .addCubeWithHalfExtent(halfExtent)
            .generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1))
            .finishMesh();

        auto actor = MakeActor<Actor> ();
        auto meshComponent = std::make_shared<ActorMeshSceneComponent> ();
        meshComponent->mesh = wallMesh;
        actor->addComponent(meshComponent);

        auto collisionShapeComponent = std::make_shared<BoxCollisionShapeComponent> ();
        collisionShapeComponent->halfExtent = halfExtent;
        actor->addComponent(collisionShapeComponent);

        auto collisionObject = std::make_shared<CollisionObjectComponent> ();
        actor->addComponent(collisionObject);

        actor->setPosition(center);
        
        world->spawnActor(actor);

    }

    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8f, 0.8f, 0.5f).normalized();
        pointLightSource->intensity = 10;
        pointLightSource->influenceRadius = 20;

        auto lightComponent = std::make_shared<ActorLightSourceComponent> ();
        lightComponent->light = pointLightSource;

        auto lightSourceActor = MakeActor<Actor> ();
        lightSourceActor->addComponent(lightComponent);
        lightSourceActor->setPosition(Vector3(0, 1.5, 0));
        world->spawnActor(lightSourceActor);
    }
    
    // Camera actor
    {
        auto cameraActor = MakeActor<Actor> ();
        auto cameraComponent = std::make_shared<ActorCameraComponent> ();
        cameraComponent->isActive = true;
        cameraActor->addComponent(cameraComponent);
        cameraActor->setPosition(Vector3(0, 1, 0));
        world->spawnActor(cameraActor);
    }

    world->playInSystemWindow();
    return 0;
}