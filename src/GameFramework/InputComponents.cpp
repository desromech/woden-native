#include "Woden/GameFramework/InputComponents.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/InputSubsystem.hpp"
#include "Woden/GameFramework/World.hpp"

namespace Woden
{
namespace GameFramework
{

void InputListenerComponent::dispatchEvent(const EventPtr &event)
{
    auto actor = owner.lock();
    if(actor)
        actor->processEvent(event);
}

void InputListenerComponent::registerInWorld(const WorldPtr &world)
{
    world->getInputSubsystem()->registerInputListener(std::static_pointer_cast<InputListenerComponent> (shared_from_this()));
}

}
}