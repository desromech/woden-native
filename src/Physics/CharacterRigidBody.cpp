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
    startTransform.translation += stepUpVector*stepUpOffset;

    auto endTransform = startTransform;
    endTransform.translation += stepUpVector*stepHeight;

    auto world = owner.lock();
    Physics::CollisionObjectExclusionSet exclusionSet;
    exclusionSet.excludedObjects.push_back(shared_from_this());

    auto sweepTestOptionalResult = world->sweepTest(shape, startTransform, endTransform, exclusionSet);
    if(!sweepTestOptionalResult.has_value())
        return;

    auto sweepTestResult = sweepTestOptionalResult.value();
    printf("stepUpMovement sweepTestResult lambda %f\n", sweepTestResult.distance);
}

void CharacterRigidBody::stepDownMovement()
{
    auto startTransform = transform;

    auto endTransform = startTransform;
    endTransform.translation = startTransform.translation - stepUpVector*stepHeight;

    auto world = owner.lock();
    Physics::CollisionObjectExclusionSet exclusionSet;
    exclusionSet.excludedObjects.push_back(shared_from_this());

    auto sweepTestOptionalResult = world->sweepTest(shape, startTransform, endTransform, exclusionSet);
    if(!sweepTestOptionalResult.has_value())
        return;

    auto sweepTestResult = sweepTestOptionalResult.value();
    printf("stepDownMovement sweepTestResult lambda %f\n", sweepTestResult.distance);
}

void CharacterRigidBody::integrateMovement(Math::Scalar deltaTime)
{
    stepUpMovement();    
    RigidBody::integrateMovement(deltaTime);
    stepDownMovement();
}

} // End of namespace Physics
} // End of namespace Woden