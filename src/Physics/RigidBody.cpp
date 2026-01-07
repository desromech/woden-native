#include "Woden/Physics/RigidBody.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"

namespace Woden
{
namespace Physics
{
void RigidBody::computeMassDistribution()
{
    // TODO: Implement this.
}

void RigidBody::resetNetForces()
{
    netForce = Math::Vector3::Zeros();
    netTorque = Math::Vector3::Zeros();
}

void RigidBody::integrateMovement(Math::Scalar deltaTime)
{
    if(mass == 0)
        return;
    
    // Integrate linear velocity
    auto linearAcceleration = owner.lock()->gravity + netForce*Math::Vector3(inverseMass);
    linearVelocity += linearAcceleration*Math::Vector3(deltaTime);
    setPosition(getPosition() + linearVelocity*Math::Vector3(deltaTime));
}

bool RigidBody::needsCollisionDetection()
{
    return mass != 0;
}

}
}