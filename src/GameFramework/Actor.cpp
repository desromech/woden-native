#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/ActorComponent.hpp"
#include "Woden/GameFramework/ActorSceneComponent.hpp"

namespace Woden
{
namespace GameFramework
{
void Actor::addComponent(const ActorComponentPtr &component)
{
    component->addedToActor(shared_from_this());
    components.push_back(component);
}

void Actor::beginPlay()
{
}

void Actor::registerWithSubsystemsInWorld(const WorldPtr &world)
{
    if(isRegisteredInWorld)
        return;
    isRegisteredInWorld = true;
    
    for(auto &component : components)
        component->registerInWorld(world);
}

Math::Vector3 Actor::getPosition() const
{
    if(!rootSceneComponent)
        return Math::Vector3::Zeros();
    return rootSceneComponent->getPosition();
}

void Actor::setPosition(const Math::Vector3 &newPosition)
{
    if(rootSceneComponent)
        rootSceneComponent->setPosition(newPosition);
}

Math::Quaternion Actor::getOrientation() const
{
    if(!rootSceneComponent)
        return Math::Quaternion::Identity();
    return rootSceneComponent->getOrientation();
}
void Actor::setOrientation(const Math::Quaternion &newOrientation)
{
    if(rootSceneComponent)
        rootSceneComponent->setOrientation(newOrientation);
}

Math::TRSTransform3D Actor::getTransform() const
{
    if(!rootSceneComponent)
        return Math::TRSTransform3D();
    return rootSceneComponent->getTransform();
}
void Actor::setTransform(const Math::TRSTransform3D &newTransform)
{
    if(rootSceneComponent)
        rootSceneComponent->setTransform(newTransform);
}

} // End of namespace GameFramework
} // End of namespace Woden