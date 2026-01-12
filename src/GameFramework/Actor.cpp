#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/ActorComponent.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"
#include "Woden/GameFramework/ActorTickSubsystem.hpp"
#include "Woden/GameFramework/World.hpp"

namespace Woden
{
namespace GameFramework
{

void Actor::setupComponents()
{
	// This is a place for setting up different actor components
}

void Actor::addComponent(const ActorComponentPtr &component)
{
    component->addedToActor(shared_from_this());
    components.push_back(component);
}

void Actor::beginPlay()
{
}

void Actor::endPlay()
{
}

void Actor::tick(float deltaTime)
{
    (void)deltaTime;
}

void Actor::registerWithSubsystemsInWorld(const WorldPtr &newWorld)
{
    if(isRegisteredInWorld)
        return;
    isRegisteredInWorld = true;

    if(wantsToTick)
        subscribeForTicking();
    
    for(auto &component : components)
        component->registerInWorld(newWorld);
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

void Actor::setWantsToTick(bool newWantsToTick)
{
    wantsToTick = newWantsToTick;
    if(isRegisteredInWorld)
    {
        if(wantsToTick)
            subscribeForTicking();
        else
            unsubscribeForTicking();
    }
}

void Actor::subscribeForTicking()
{
    world.lock()->getActorTickSubsystem()->subscribe(shared_from_this());
}

void Actor::unsubscribeForTicking()
{
    world.lock()->getActorTickSubsystem()->unsubscribe(shared_from_this());
}

void Actor::processEvent(const EventPtr &event)
{
    event->sentToActor(shared_from_this());
}

void Actor::handleUnknownEvent(const EventPtr &event)
{
    (void)event;
}
void Actor::handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    (void)event;
}
void Actor::handleMouseButtonUpEvent(const MouseButtonUpEventPtr &event)
{
    (void)event;
}
void Actor::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    (void)event;
}
void Actor::handleMouseWheelEvent(const MouseWheelEventPtr &event)
{
    (void)event;
}

} // End of namespace GameFramework
} // End of namespace Woden