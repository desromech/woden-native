#include "Woden/GameFramework/CharacterActor.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"
#include "Woden/GameFramework/CollisionShapeComponents.hpp"
#include "Woden/GameFramework/CollisionObjectComponents.hpp"

namespace Woden
{
namespace GameFramework
{

CharacterActor::CharacterActor()
{
    setWantsToTick(true);
}

CharacterActor::~CharacterActor()
{
}

void CharacterActor::tick(Math::Scalar deltaTime)
{
    (void)deltaTime;
    auto walkingVelocity = walkingDirection.normalized() * Math::Vector3(walkingSpeed);
    auto bodyComponent = std::static_pointer_cast<CharacterBodyObjectComponent> (collisionObjectComponent);
    bodyComponent->walkingVelocity = walkingVelocity;
}

void CharacterActor::jump()
{
    auto bodyComponent = std::static_pointer_cast<CharacterBodyObjectComponent> (collisionObjectComponent);
    bodyComponent->jump();

}

void CharacterActor::setupComponents()
{
    Actor::setupComponents();

    addComponent(std::make_shared<ActorSceneComponent> ());

    {
        auto capsuleShape = std::make_shared<CapsuleYCollisionShapeComponent> ();
        capsuleShape->height = getDefaultHeight() - getDefaultBroadness();
        capsuleShape->radius = getDefaultBroadness() * 0.5f;
        addComponent(capsuleShape);
    }
    
    {
        auto bodyComponent = std::make_shared<CharacterBodyObjectComponent> ();
        bodyComponent->setMass(getDefaultMass());
        bodyComponent->restitutionCoefficient = 0.0f;
        bodyComponent->withoutTorque();
        bodyComponent->jumpVelocity = getDefaultJumpVelocity();
        addComponent(bodyComponent);
    }

}

} // End of namespace GameFramework
} // End of namespace Woden