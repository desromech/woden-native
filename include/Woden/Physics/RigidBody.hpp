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

    virtual Math::Scalar getMass() const override
    {
        return mass;
    }

    virtual Math::Scalar getInverseMass() const override
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
    virtual void applyMovementAtRelativePoint(Math::Scalar movement, const Math::Vector3 &relativePoint, const Math::Vector3 &normalDirection) override;
    virtual void applyImpulse(Math::Vector3 impulse);

protected:
    Math::Scalar mass = 0;
    Math::Scalar inverseMass = 0;
    Math::Matrix3x3 inertiaTensor = Math::Matrix3x3::Zeros();

    Math::Vector3 netForce = Math::Vector3::Zeros();
    Math::Vector3 netTorque = Math::Vector3::Zeros();
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_RIGID_BODY_HPP