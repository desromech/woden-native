#include "Woden/GameFramework/ActorTickSubsystem.hpp"
#include "Woden/GameFramework/Actor.hpp"

namespace Woden
{
namespace GameFramework
{

void ActorTickSubsystem::prePhysicsUpdateSingleTimeStep(Math::Scalar deltaTime)
{
    for(auto &actor : tickingActors)
        actor->tick(deltaTime);
}

void ActorTickSubsystem::subscribe(const ActorPtr &actor)
{
    tickingActors.push_back(actor);
}

void ActorTickSubsystem::unsubscribe(const ActorPtr &actor)
{
    for(size_t i = 0; i < tickingActors.size(); ++i)
    {
        if(tickingActors[i] == actor)
        {
            tickingActors.erase(tickingActors.begin() + i);
            return;
        }
    }
}

} // End of namespace GameFramework
} // End of namespace Woden
