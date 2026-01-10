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

    // Box grid
    for(int x = -2; x <= 2; ++x)
    {
        for(int z = -2; z <= 2; ++z)
        {
            auto shape = std::make_shared<BoxCollisionShape> ();
            shape->setHalfExtent(Vector3(0.5));

            auto rigidBody = std::make_shared<RigidBody> ();
            rigidBody->shape = shape;
            rigidBody->setMass(1);
            rigidBody->computeMassDistribution();
            rigidBody->setPosition(Vector3(x*1.5f, 1.0f, z*1.5f));

            world->addCollisionObject(rigidBody);

        }
    }


    // Floor
    {
        auto shape = std::make_shared<BoxCollisionShape> ();
        shape->setHalfExtent(Vector3(4, 0.1f, 4));

        auto floor = std::make_shared<RigidBody> ();
        floor->shape = shape;
        floor->setMass(0);
        floor->computeMassDistribution();
        floor->setPosition(Vector3(0, -0.8f, 0));

        world->addCollisionObject(floor);
    }


    world->buildInteractiveScene()->openInSystemWindow();
    return 0;
}