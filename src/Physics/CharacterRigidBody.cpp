#include "Woden/Physics/CharacterRigidBody.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"
#include <stdio.h>

namespace Woden
{
namespace Physics
{

void CharacterRigidBody::stepUpMovement()
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

void CharacterRigidBody::stepDownMovement()
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

void CharacterRigidBody::integrateMovement(Math::Scalar deltaTime)
{
    stepUpMovement();
    RigidBody::integrateMovement(deltaTime);
    stepDownMovement();
    //printf("Linear velocity %f %f %f\n", linearVelocity.x, linearVelocity.y, linearVelocity.z);
    //printf("On floor %d\n", isCurrentlyOnFloor);
}

} // End of namespace Physics
} // End of namespace Woden