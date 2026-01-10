#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"
#include "Woden/Physics/RigidBody.hpp"
#include "Woden/Physics/RopeConstraint.hpp"

using namespace Woden::Math;
using namespace Woden::Physics;
using namespace Woden::SceneGraph;

int woden_main(int argc, const char **argv)
{
    (void)argc;
    (void)argv;
    auto world = std::make_shared<DiscreteDynamicsPhysicsWorld> ();

    // Suspended box
    auto shape = std::make_shared<BoxCollisionShape> ();
    shape->setHalfExtent(Vector3(0.5));

    auto suspendedBox = std::make_shared<RigidBody> ();
    suspendedBox->shape = shape;
    suspendedBox->setMass(1);
    suspendedBox->computeMassDistribution();
    suspendedBox->setPosition(Vector3(1, 1, 0));

    world->addCollisionObject(suspendedBox);

    // Anchor box
    shape = std::make_shared<BoxCollisionShape> ();
    shape->setHalfExtent(Vector3(0.1f));

    auto anchorBox = std::make_shared<RigidBody> ();
    anchorBox->shape = shape;
    anchorBox->setMass(0);
    anchorBox->setPosition(Vector3(0, 1, 0));

    world->addCollisionObject(anchorBox);

    // Rope constraint
    auto rope = std::make_shared<RopeConstraint> ();
    rope->firstObject = anchorBox;
    rope->secondObject = suspendedBox;
    rope->secondObjectLocalAnchorPoint = Vector3(-0.5, 0, 0);
    world->addForceGenerator(rope);

    world->buildInteractiveScene()->openInSystemWindow();
    return 0;
}