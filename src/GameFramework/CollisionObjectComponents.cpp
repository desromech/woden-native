#include "Woden/GameFramework/CollisionObjectComponents.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/CollisionShapeComponents.hpp"
#include "Woden/GameFramework/PhysicsSubsystem.hpp"
#include "Woden/GameFramework/World.hpp"
#include "Woden/Physics/RigidBody.hpp"
#include <stdlib.h>

namespace Woden
{
namespace GameFramework
{

void CollisionObjectComponent::addedToActor(const ActorPtr &actor)
{
    ActorComponent::addedToActor(actor);
    actor->collisionObjectComponent = std::static_pointer_cast<CollisionObjectComponent> (shared_from_this());
}

void CollisionObjectComponent::registerInWorld(const WorldPtr &world)
{
    ActorComponent::registerInWorld(world);

    collisionObject = makeCollisionObjectInstance();
    world->getPhysicsSubsystem()->physicsWorld->addCollisionObject(collisionObject);

}

void CollisionObjectComponent::setupCollisionObject(const Physics::CollisionObjectPtr &collisionObject)
{
    auto ownerActor = owner.lock();
    if(!ownerActor || !ownerActor->collisionShapeComponent)
    {
        fprintf(stderr, "TODO: collision object without shape.\n");
        abort();
    }

    collisionObject->shape = ownerActor->collisionShapeComponent->asValidCollisionShape();
    collisionObject->model = this;
}

Physics::CollisionObjectPtr CollisionObjectComponent::makeCollisionObjectInstance()
{
    auto object = std::make_shared<Physics::CollisionObject> ();
    setupCollisionObject(object);
    return object;
}

void CollisionObjectComponent::loadCollisionStateInto(const Physics::CollisionObjectPtr &collisionObject)
{
    auto actor = owner.lock();
    if(actor)
    {
        auto transform = actor->getTransform();
        Math::RigidTransform rigidTransform;
        rigidTransform.rotation = transform.rotation;
        rigidTransform.translation = transform.translation;
        collisionObject->setTransform(rigidTransform);
    }
}

void CollisionObjectComponent::saveCollisionStateFrom(const Physics::CollisionObjectPtr &collisionObject)
{
    auto actor = owner.lock();
    if(actor)
    {
        actor->setTransform(collisionObject->getTransform().asTRSTransform3D());
    }
}

Physics::CollisionObjectPtr RigidBodyComponent::makeCollisionObjectInstance()
{
    auto body = std::make_shared<Physics::RigidBody> ();
    setupCollisionObject(body);
    body->setMass(mass);
    if(noTorque)
        body->setInertiaTensor(Math::Matrix3x3::Zeros());
    else
        body->computeMassDistribution();
    return body;
}


} // End of namespace GameFramework
} // End of namespace Woden
