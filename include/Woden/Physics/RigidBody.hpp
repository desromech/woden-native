#ifndef WODEN_PHYSICS_RIGID_BODY_HPP
#define WODEN_PHYSICS_RIGID_BODY_HPP

#include "CollisionObject.hpp"
#include "Woden/Math/Matrix3x3.hpp"
namespace Woden
{
namespace Physics
{
/**
 * I am an object that participates in colliosions.
 */
class RigidBody : public CollisionObject
{
public:

    Math::Scalar getMass() const
    {
        return mass;
    }

    Math::Scalar getInverseMass() const
    {
        return inverseMass;
    }

    void setMass(Math::Scalar newMass)
    {
        mass = newMass;
        inverseMass = mass == 0 ? 0 : (1/newMass);
    }


    void computeMassDistribution();
    
    void resetNetForces() override;
    void integrateMovement(Math::Scalar deltaTime) override;
    
    bool needsCollisionDetection() override;

protected:
    Math::Scalar mass = 0;
    Math::Scalar inverseMass = 0;
    Math::Matrix3x3 inertiaTensor = Math::Matrix3x3::Zeros();

    Math::Vector3 netForce = Math::Vector3::Zeros();
    Math::Vector3 netTorque = Math::Vector3::Zeros();

    Math::Vector3 linearVelocity;
    Math::Vector3 angularVelocity;
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_RIGID_BODY_HPP