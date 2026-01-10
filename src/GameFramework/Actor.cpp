#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/ActorComponent.hpp"

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

} // End of namespace GameFramework
} // End of namespace Woden