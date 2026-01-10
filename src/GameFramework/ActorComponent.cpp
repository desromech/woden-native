#include "Woden/GameFramework/ActorComponent.hpp"
#include <assert.h>

namespace Woden
{
namespace GameFramework
{

void ActorComponent::addedToActor(const ActorPtr &actor)
{
    assert(!owner.lock());
    owner = actor;
}

void ActorComponent::registerInWorld(const WorldPtr &world)
{
    (void)world;
}

} // End of namespace GameFramework
} // End of namespace Woden