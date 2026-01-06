#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"
#include "Woden/Physics/RigidBody.hpp"

using namespace Woden::Math;
using namespace Woden::Physics;
using namespace Woden::SceneGraph;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto world = std::make_shared<DiscreteDynamicsPhysicsWorld> ();

    // Falling box
    {
        auto shape = std::make_shared<BoxCollisionShape> ();
        shape->setHalfExtent(Vector3(0.5));

        auto rigidBody = std::make_shared<RigidBody> ();
        rigidBody->shape = shape;
        rigidBody->setMass(1);
        rigidBody->computeMassDistribution();
        rigidBody->setPosition(Vector3(0, 1, 0));

        world->addCollisionObject(rigidBody);
    }

    // Floor
    {
        auto shape = std::make_shared<BoxCollisionShape> ();
        shape->setHalfExtent(Vector3(2, 0.1, 2));

        auto floor = std::make_shared<RigidBody> ();
        floor->shape = shape;
        floor->setMass(0);
        floor->computeMassDistribution();
        floor->setPosition(Vector3(0, -0.8, 0));

        world->addCollisionObject(floor);
    }


    world->buildInteractiveScene()->openInSystemWindow();
    return 0;
}