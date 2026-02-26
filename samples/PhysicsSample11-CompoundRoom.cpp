#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"
#include "Woden/Physics/RigidBody.hpp"
#include "Woden/Rendering/LightSource.hpp"

using namespace Woden::Math;
using namespace Woden::Physics;
using namespace Woden::SceneGraph;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto world = std::make_shared<DiscreteDynamicsPhysicsWorld> ();

    auto compoundShape = std::make_shared<CompoundCollisionShape> ();

    AABox walls[] = {
        AABox(Vector3(-10, -0.5, -10), Vector3(10, 0.0, 10)),
		AABox(Vector3(-10,  5.0, -10), Vector3(10, 5.0, 10)),

		AABox(Vector3(-10, 0,  10), Vector3(10, 5.0, 10)), 
		AABox(Vector3(-10, 0, -10), Vector3(10, 5.0, -10)),

		AABox(Vector3(-10, 0, -10), Vector3(-10, 5.0, 10)),
		AABox(Vector3( 10, 0, -10), Vector3( 10, 5.0, 10)),
        
		AABox(Vector3(-1, 0, -6), Vector3(1, 1.0, -5)),
    };

    for(auto &wall : walls)
    {
        auto center = wall.center();
        auto halfExtent = wall.halfExtent();

        auto shape = std::make_shared<BoxCollisionShape> ();
        shape->setHalfExtent(halfExtent);
        compoundShape->addChildWithTranslation(shape, center);
    }

    compoundShape->finishAddingChildren();
    auto roomWalls = std::make_shared<RigidBody> ();
    roomWalls->shape = compoundShape;
    roomWalls->setMass(0);
    roomWalls->computeMassDistribution();
    roomWalls->setPosition(Woden::Math::Vector3::Zeros());

    world->addCollisionObject(roomWalls);

    // Falling boxes
    Woden::Math::Vector3 boxPositions[] = {
        Woden::Math::Vector3(-1, 0.5, -3),
		Woden::Math::Vector3( 0, 0.5, -3),
		Woden::Math::Vector3( 1, 0.5, -3),
    };
    for(auto &boxPosition : boxPositions)
    {
        auto halfExtent = Vector3(0.25);
        auto shape = std::make_shared<BoxCollisionShape> ();
        shape->margin = 0.1;
        shape->setHalfExtent(halfExtent);

        auto rigidBody = std::make_shared<RigidBody> ();
        rigidBody->shape = shape;
        rigidBody->setMass(50);
        rigidBody->computeMassDistribution();
        rigidBody->setPosition(boxPosition);

        world->addCollisionObject(rigidBody);
    }

    auto scene = world->buildInteractiveScene();
    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8f, 0.8f, 0.7f);
        pointLightSource->intensity = 10;
        pointLightSource->influenceRadius = 20;
        pointLightSource->castShadows = true;

        scene->normalLayer->addChild(pointLightSource->asSceneNodeWithPosition(Vector3(0, 1.5, 0)));
    }

    scene->openInSystemWindow();
    return 0;
}