#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"
#include "Woden/GameFramework/CollisionObjectComponents.hpp"
#include "Woden/GameFramework/CollisionShapeComponents.hpp"
#include "Woden/GameFramework/PlayerActor.hpp"
#include "Woden/GameFramework/World.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
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
    Woden::Rendering::MetallicRoughnessMaterialPtr ceilingMaterial = std::make_shared<Woden::Rendering::MetallicRoughnessMaterial> ();
    ceilingMaterial->baseColorFactor = Woden::Math::Vector4(0.05, 0.05, 0.05, 1.0);
    ceilingMaterial->roughnessFactor = 0.8f;
    ceilingMaterial->metallicFactor = 0.0f;

    Woden::Rendering::MetallicRoughnessMaterialPtr wallMaterial = std::make_shared<Woden::Rendering::MetallicRoughnessMaterial> ();
    wallMaterial->baseColorFactor = Woden::Math::Vector4(0.8f, 0.8f, 0.8f, 1.0);
    wallMaterial->roughnessFactor = 0.8f;
    wallMaterial->metallicFactor = 0.0f;

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
    Woden::Math::Vector3 boxPositions[] = {
        Woden::Math::Vector3(-1, 0.5, -3),
		Woden::Math::Vector3( 0, 0.5, -3),
		Woden::Math::Vector3( 1, 0.5, -3),
    };
    for(auto &boxPosition : boxPositions)
    {
        auto halfExtent = Vector3(0.25);
        auto cubeMesh = Woden::Rendering::MeshBuilder()
            .addCubeWithHalfExtent(halfExtent)
            .generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1))
            .finishMesh();
        
        auto actor = MakeActor<Actor> ();
        auto meshComponent = std::make_shared<ActorMeshSceneComponent> ();
        meshComponent->mesh = cubeMesh;
        actor->addComponent(meshComponent);

        auto collisionShapeComponent = std::make_shared<BoxCollisionShapeComponent> ();
        collisionShapeComponent->halfExtent = halfExtent;
        actor->addComponent(collisionShapeComponent);

        auto rigidBody = std::make_shared<RigidBodyComponent> ();
        rigidBody->setMass(1);
        actor->addComponent(rigidBody);

        actor->setPosition(boxPosition);

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

    // Player actor
    {
        auto playerActor = MakeActor<PlayerActor> ();
        playerActor->setPosition(Vector3(0, 2, 0));
        world->spawnActor(playerActor);
    }

    world->playInSystemWindow();
    return 0;
}