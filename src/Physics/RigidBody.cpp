#include "Woden/Physics/RigidBody.hpp"
#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"

namespace Woden
{
namespace Physics
{
void RigidBody::computeMassDistribution()
{
    if(mass == 0)
        setInertiaTensor(Math::Matrix3x3::Zeros());
    else
        setInertiaTensor(shape->computeInertiaTensorWithMass(mass));

}

Math::Matrix3x3 RigidBody::computeVelocityPerImpulseWorldMatrixForRelativeContactPoint(const Math::Vector3 &relativePoint) const
{
    auto relativePointCrossMatrix = Math::Matrix3x3::SkewSymmetric(relativePoint);
    auto torquePerUnitImpulse = relativePointCrossMatrix;
    auto rotationPerUnitImpulse = worldInverseInertiaTensor * torquePerUnitImpulse;
    return -(relativePointCrossMatrix * rotationPerUnitImpulse);
}

Math::Vector3 RigidBody::computeVelocityAtRelativePoint(const Math::Vector3 &relativePoint)
{
    return linearVelocity + angularVelocity.cross(relativePoint);
}

void RigidBody::setInertiaTensor(const Math::Matrix3x3 &tensor)
{
    inertiaTensor = tensor;
    if(inertiaTensor.determinant() == 0)
        inverseInertiaTensor = Math::Matrix3x3::Zeros();
    else
        inverseInertiaTensor = inertiaTensor.inverse();
    updateWorldInertiaTensor();
}

void RigidBody::updateWorldInertiaTensor()
{
    auto rotationMatrix = transform.rotation.asMatrix();
    auto transposedRotationMatrix = transform.rotation.asMatrix();

	worldInertiaTensor = rotationMatrix * inertiaTensor * transposedRotationMatrix;
	worldInverseInertiaTensor = rotationMatrix * inverseInertiaTensor * transposedRotationMatrix;
}

void RigidBody::transformChanged()
{
    CollisionObject::transformChanged();
    updateWorldInertiaTensor();
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
    
    // Compute the linear acceleration
    auto linearAcceleration = owner.lock()->gravity + netForce*Math::Vector3(inverseMass) - linearVelocity*Math::Vector3(linearDamping) + internalLinearAcceleration;

    // Integrate the linear acceleration
    linearVelocity += linearAcceleration*Math::Vector3(deltaTime);

    // Compute the angular acceleration.
    auto angularAcceleration = worldInverseInertiaTensor * netTorque - angularVelocity*Math::Vector3(angularDamping);

    // Integrate the angular acceleration.
    angularVelocity += angularAcceleration*Math::Vector3(deltaTime);

    // Integrate the position.
    auto integratedPosition = getPosition() + linearVelocity*Math::Vector3(deltaTime);

    // Integrate the orientation
    auto integratedOrientation = Math::Quaternion(angularVelocity*Math::Vector3(0.5*deltaTime)).exp() * getOrientation();
    integratedOrientation = integratedOrientation.normalized();

    setPositionAndOrientation(integratedPosition, integratedOrientation);

}

bool RigidBody::needsCollisionDetection()
{
    return mass != 0;
}

void RigidBody::applyMovementAtRelativePoint(Math::Scalar movement, const Math::Vector3 &relativePoint, const Math::Vector3 &normalDirection)
{
    (void)relativePoint;
    auto linearMovement = movement *inverseMass;

    setPosition(getPosition() + normalDirection*Math::Vector3(linearMovement));
}

void RigidBody::applyImpulse(const Math::Vector3 &impulse)
{
    linearVelocity += impulse*Math::Vector3(inverseMass);
}

void RigidBody::applyImpulseInRelativePosition(const Math::Vector3 &impulse, const Math::Vector3 &relativePoint)
{
    linearVelocity += impulse*inverseMass;
	angularVelocity += worldInverseInertiaTensor * (relativePoint.cross(impulse));
}

}
}