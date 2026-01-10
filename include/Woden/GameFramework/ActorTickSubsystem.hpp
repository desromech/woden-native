#ifndef WODEN_GAME_FRAMEWORK_ACTOR_TICK_SUBSYSTEM_HPP
#define WODEN_GAME_FRAMEWORK_ACTOR_TICK_SUBSYSTEM_HPP

#include "Subsystem.hpp"
#include <vector>

namespace Woden
{
namespace GameFramework
{

typedef std::shared_ptr<class Actor> ActorPtr;

/**
 * I am a subsystem that deals with dispatching the tick event.
 */
class ActorTickSubsystem : public Subsystem 
{
public:
    virtual void prePhysicsUpdateSingleTimeStep(Math::Scalar deltaTime) override;

    void subscribe(const ActorPtr &actor);
    void unsubscribe(const ActorPtr &actor);

    std::vector<ActorPtr> tickingActors;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_ACTOR_TICK_SUBSYSTEM_HPP
