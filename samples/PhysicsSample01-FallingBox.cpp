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

    {
        auto shape = std::make_shared<BoxCollisionShape> ();
        shape->setHalfExtent(Vector3(0.5));

        auto rigidBody = std::make_shared<RigidBody> ();
        rigidBody->shape = shape;
        rigidBody->mass = 1;
        rigidBody->computeMassDistribution();
        rigidBody->setPosition(Vector3(0, 1, 0));

        world->addCollisionObject(rigidBody);
    }

    world->buildInteractiveScene()->openInSystemWindow();
    return 0;
}