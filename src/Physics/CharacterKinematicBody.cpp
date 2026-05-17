#include "Woden/Physics/CharacterKinematicBody.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"
#include <stdio.h>

namespace Woden
{
namespace Physics
{

void CharacterKinematicBody::stepUpMovement()
{
    auto startTransform = transform;
    startTransform.translation += stepUpVector*Math::Vector3(stepUpOffset);

    auto endTransform = startTransform;
    endTransform.translation += stepUpVector*Math::Vector3(stepHeight);
    //auto deltaTranslation = endTransform.translation - startTransform.translation;
    //printf("stepUp delta %f %f %f\n", deltaTranslation.x, deltaTranslation.y, deltaTranslation.z);

    auto world = owner.lock();
    Physics::CollisionObjectExclusionSet exclusionSet;
    exclusionSet.excludedObjects.push_back(shared_from_this());

    auto sweepTestOptionalResult = world->sweepTest(shape, startTransform, endTransform, exclusionSet);
    if(!sweepTestOptionalResult.has_value())
    {
        //printf("stepUpMovement endTransform %f %f %f\n", endTransform.translation.x, endTransform.translation.y, endTransform.translation.z);
        setTransform(endTransform);
        return;
    }

    auto sweepTestResult = sweepTestOptionalResult.value();
    auto lambda = sweepTestResult.distance;
    
    auto sweepTransform = startTransform.interpolateTo(endTransform, lambda);
    //printf("stepUpMovement lambda %f transform %f %f %f\n", lambda, sweepTransform.translation.x, sweepTransform.translation.y, sweepTransform.translation.z);

    setTransform(sweepTransform);
}

void CharacterKinematicBody::stepDownMovement()
{
    auto startTransform = transform;

    auto endTransform = startTransform;
    endTransform.translation = startTransform.translation - stepUpVector*Math::Vector3(stepHeight + stepUpOffset);
    //auto deltaTranslation = endTransform.translation - startTransform.translation;
    //printf("stepDown delta %f %f %f\n", deltaTranslation.x, deltaTranslation.y, deltaTranslation.z);

    auto world = owner.lock();
    Physics::CollisionObjectExclusionSet exclusionSet;
    exclusionSet.excludedObjects.push_back(shared_from_this());

    auto sweepTestOptionalResult = world->sweepTest(shape, startTransform, endTransform, exclusionSet);
    if(!sweepTestOptionalResult.has_value())
    {
        //printf("stepDownMovement endTransform %f %f %f\n", endTransform.translation.x, endTransform.translation.y, endTransform.translation.z);
        isCurrentlyOnFloor = false;
        setTransform(endTransform);
        return;
    }

    auto sweepTestResult = sweepTestOptionalResult.value();
    auto lambda = sweepTestResult.distance;

    auto sweepTransform = startTransform.interpolateTo(endTransform, lambda);
    auto NdotUp = sweepTestResult.normal.dot(stepUpVector);

    //printf("NdotUp %f N %f %f %f\n", NdotUp, sweepTestResult.normal.x, sweepTestResult.normal.y, sweepTestResult.normal.z);
    linearVelocity = linearVelocity - (sweepTestResult.normal * Math::Vector3(linearVelocity.dot(sweepTestResult.normal)));

    //printf("stepDownMovement lambda %f transform %f %f %f\n", lambda, sweepTransform.translation.x, sweepTransform.translation.y, sweepTransform.translation.z);
    isCurrentlyOnFloor = NdotUp >= 0.6;
    setTransform(sweepTransform);
}

void CharacterKinematicBody::integrateBodyMovement(Math::Scalar deltaTime)
{
    auto startLinearVelocity = linearVelocity;

    // Compute the linear acceleration
    auto linearAcceleration = owner.lock()->gravity + internalLinearAcceleration;

    // Integrate the linear acceleration
    auto integratedVelocity = linearVelocity + linearAcceleration*Math::Vector3(deltaTime);

    // Apply the linear damping
    auto linearDampingFactor = Math::clamp(powf(1.0f - linearDamping, deltaTime), 0.0f, 1.0f);
    integratedVelocity *= linearDampingFactor;

    // Integrate the linear velocity
    linearVelocityIntegrationDelta = integratedVelocity - linearVelocity;
    linearVelocity = integratedVelocity;

    // Compute the angular acceleration.
    auto angularAcceleration = Math::Vector3::Zeros();

    // Integrate the angular acceleration.
    auto integratedAngularVelocity = angularVelocity + angularAcceleration*Math::Vector3(deltaTime);

    // Apply the angular damping
    auto angularDampingFactor = Math::clamp(powf(1.0f - angularDamping, deltaTime), 0.0f, 1.0f);
    integratedAngularVelocity *= angularDampingFactor;

    // Integrat the angular velocity
    angularVelocityIntegrationDelta = integratedAngularVelocity - angularVelocity;
    angularVelocity = integratedAngularVelocity;

    // Integrate the position.
    auto integratedPosition = getPosition() + linearVelocity*Math::Vector3(deltaTime);

    // Integrate the orientation
    auto integratedOrientation = Math::Quaternion(angularVelocity*Math::Vector3(0.5f*deltaTime)).exp() * getOrientation();
    integratedOrientation = integratedOrientation.normalized();

    // Start transform
    auto startTransform = getTransform();
    auto endTransform = Math::RigidTransform::WithRotationAndTranslation(integratedOrientation, integratedPosition);

    auto world = owner.lock();
    Physics::CollisionObjectExclusionSet exclusionSet;
    exclusionSet.excludedObjects.push_back(shared_from_this());

    auto sweepTestOptionalResult = world->sweepTest(shape, startTransform, endTransform, exclusionSet);
    if(!sweepTestOptionalResult.has_value())
    {
        setTransform(endTransform);
        return;
    }

    auto sweepTestResult = sweepTestOptionalResult.value();
    auto lambda = sweepTestResult.distance;

    auto sweepTransform = startTransform.interpolateTo(endTransform, lambda);

    auto endLinearVelocity = linearVelocity;
    linearVelocity = Math::mix(startLinearVelocity, endLinearVelocity, lambda);
    
    //linearVelocity = linearVelocity - (sweepTestResult.normal * linearVelocity.dot(sweepTestResult.normal));

    //printf("Sweep translation: %f %f %f\n", sweepTransform.translation.x, sweepTransform.translation.y, sweepTransform.translation.z);
    printf("lambda: %f\n", lambda);
    printf("Normal: %f %f %f\n", sweepTestResult.normal.x, sweepTestResult.normal.y, sweepTestResult.normal.z);
    printf("LinearVelocity: %f %f %f\n", linearVelocity.x, linearVelocity.y, linearVelocity.z);

    setTransform(sweepTransform);
}

void CharacterKinematicBody::integrateMovement(Math::Scalar deltaTime)
{
    //printf("CharacterKinematicBody::integrateMovement\n");
    stepUpMovement();
    integrateBodyMovement(deltaTime);
    stepDownMovement();
    //printf("Linear velocity %f %f %f\n", linearVelocity.x, linearVelocity.y, linearVelocity.z);
    //printf("On floor %d\n", isCurrentlyOnFloor);
}

} // End of namespace Physics
} // End of namespace Woden