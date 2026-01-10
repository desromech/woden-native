#include "Woden/GameFramework/PhysicsSubsystem.hpp"

namespace Woden
{
namespace GameFramework
{

PhysicsSubsystem::PhysicsSubsystem()
{
    physicsWorld = std::make_shared<Physics::DiscreteDynamicsPhysicsWorld> ();
}

PhysicsSubsystem::~PhysicsSubsystem()
{
}

void PhysicsSubsystem::updateSingleTimeStep(Math::Scalar deltaTime)
{
    physicsWorld->loadCollisionStateFromModels();
    physicsWorld->updateSingleTimeStep(deltaTime);
    physicsWorld->saveCollisionStateIntoModels();
}

} // End of namespace GameFramework
} // End of namespace Woden
