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
    RigidBody();
    ~RigidBody();

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

    const Math::Matrix3x3 &getInertiaTensor() const
    {
        return inertiaTensor;
    }

    const Math::Matrix3x3 &getInverseInertiaTensor() const
    {
        return inverseInertiaTensor;
    }

    void setInertiaTensor(const Math::Matrix3x3 &tensor);

    void computeMassDistribution();
    
    virtual Math::Scalar computeAngularInertiaForRelativeContactPoint(const Math::Vector3 &relativePoint, const Math::Vector3 &normal) const override;
    virtual Math::Matrix3x3 computeVelocityPerImpulseWorldMatrixForRelativeContactPoint(const Math::Vector3 &relativePoint) const override;
    virtual Math::Vector3 computeVelocityAtRelativePoint(const Math::Vector3 &relativePoint);

    virtual void transformChanged() override;

    void resetNetForces() override;
    void integrateMovement(Math::Scalar deltaTime) override;
    
    bool needsCollisionDetection() override;

    virtual void applyForceInRelativePosition(const Math::Vector3 &force, const Math::Vector3 &relativePosition);
    virtual void applyMovementAtRelativePoint(Math::Scalar movement, const Math::Vector3 &relativePoint, const Math::Vector3 &normalDirection) override;
    virtual void applyImpulse(const Math::Vector3 &impulse);
    virtual void applyImpulseInRelativePosition(const Math::Vector3 &impulse, const Math::Vector3 &relativePoint);

    void wakeUpForTranslationBy(const Math::Vector3 &linearTranslation);
    void wakeUpForTranslationByAndRotateByAngularIncrement(const Math::Vector3 &linearIncrement, const Math::Vector3 &angularIncrement);

    void setSleepingStateFactors();

    virtual void resetSleepingState();
    virtual void wakeUp() override;

    void checkTimeToSleep(Math::Scalar weight);

    bool isSleeping()
    {
        return !isAwake;
    }
    
    Math::Scalar computeMovementAmount() const
    {
        return linearVelocity.length2() + angularVelocity.length2();
    }

    Math::Scalar linearDamping = 0.2f;
    Math::Scalar angularDamping = 0.2f;

    Math::Scalar angularMovementLimit = 0.2f;

protected:
    void updateWorldInertiaTensor();

    Math::Scalar mass = 0;
    Math::Scalar inverseMass = 0;
    Math::Matrix3x3 inertiaTensor = Math::Matrix3x3::Zeros();
    Math::Matrix3x3 inverseInertiaTensor = Math::Matrix3x3::Zeros();
    Math::Matrix3x3 worldInertiaTensor = Math::Matrix3x3::Zeros();
    Math::Matrix3x3 worldInverseInertiaTensor = Math::Matrix3x3::Zeros();

    Math::Vector3 netForce = Math::Vector3::Zeros();
    Math::Vector3 netTorque = Math::Vector3::Zeros();

    bool isAwake = false;
    Math::Scalar averageMovementAmount = 0;
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_RIGID_BODY_HPP