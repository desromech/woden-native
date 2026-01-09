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

Math::Scalar RigidBody::computeAngularInertiaForRelativeContactPoint(const Math::Vector3 &relativePoint, const Math::Vector3 &normal) const
{
    auto torquePerUnitImpulse = relativePoint.cross(normal);
	auto rotationPerUnitImpulse = worldInverseInertiaTensor * torquePerUnitImpulse;
	return (rotationPerUnitImpulse.cross(relativePoint)).dot(normal);
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
    auto transposedRotationMatrix = rotationMatrix.transpose();

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
    auto integratedVelocity = linearVelocity + linearAcceleration*Math::Vector3(deltaTime);
    linearVelocityIntegrationDelta = integratedVelocity - linearVelocity;
    linearVelocity = integratedVelocity;

    // Compute the angular acceleration.
    auto angularAcceleration = worldInverseInertiaTensor * netTorque - angularVelocity*Math::Vector3(angularDamping);

    // Integrate the angular acceleration.
    auto integratedAngularVelocity = angularVelocity + angularAcceleration*Math::Vector3(deltaTime);
    angularVelocityIntegrationDelta = integratedAngularVelocity - angularVelocity;
    angularVelocity = integratedAngularVelocity;

    // Integrate the position.
    auto integratedPosition = getPosition() + linearVelocity*Math::Vector3(deltaTime);

    // Integrate the orientation
    auto integratedOrientation = Math::Quaternion(angularVelocity*Math::Vector3(0.5f*deltaTime)).exp() * getOrientation();
    integratedOrientation = integratedOrientation.normalized();

    setPositionAndOrientation(integratedPosition, integratedOrientation);

}

bool RigidBody::needsCollisionDetection()
{
    return mass != 0;
}

void RigidBody::applyMovementAtRelativePoint(Math::Scalar movement, const Math::Vector3 &relativePoint, const Math::Vector3 &movementDirection)
{
    (void)relativePoint;
    auto linearMovement = movement *inverseMass;

    auto angularDirection = worldInverseInertiaTensor * relativePoint.cross(movementDirection);
    auto angularFactor = angularDirection.length();
    if(Math::closeTo(angularFactor, 0))
        return wakeUpForTranslationBy(movementDirection*Math::Vector3(linearMovement));

    angularDirection = angularDirection / angularFactor;
    auto angularMovement = movement*angularFactor;
    if(abs(angularMovement) <= 0)
        return wakeUpForTranslationBy(movementDirection*Math::Vector3(linearMovement));

    if(abs(angularMovement) > angularMovementLimit)
    {
        angularMovement = angularMovement >= 0 ? angularMovementLimit : -angularMovementLimit;
        auto angularSpentMovement = angularMovement / angularFactor; 

        linearMovement = (movement - angularSpentMovement)*inverseMass;
    }

    wakeUpForTranslationByAndRotateByAngularIncrement(movementDirection * Math::Vector3(linearMovement), angularDirection*Math::Vector3(angularMovement));
}

void RigidBody::wakeUpForTranslationBy(const Math::Vector3 &linearTranslation)
{
    // TODO: Wake up
    translateBy(linearTranslation);
}

void RigidBody::wakeUpForTranslationByAndRotateByAngularIncrement(const Math::Vector3 &linearIncrement, const Math::Vector3 &angularIncrement)
{
    // TODO: Wake up
    translateByAndRotateBy(linearIncrement, angularIncrement);
}

void RigidBody::applyImpulse(const Math::Vector3 &impulse)
{
    linearVelocity += impulse*Math::Vector3(inverseMass);
}

void RigidBody::applyImpulseInRelativePosition(const Math::Vector3 &impulse, const Math::Vector3 &relativePoint)
{
    linearVelocity += impulse* Math::Vector3(inverseMass);
	angularVelocity += worldInverseInertiaTensor * (relativePoint.cross(impulse));
}

}
}