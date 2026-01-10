#ifndef WODEN_GAME_FRAMEWORK_ACTOR_COMPONENT_HPP
#define WODEN_GAME_FRAMEWORK_ACTOR_COMPONENT_HPP

#include <memory>

namespace Woden
{
namespace GameFramework
{
typedef std::weak_ptr<class Actor> ActorWeakPtr;
typedef std::shared_ptr<class Actor> ActorPtr;
typedef std::shared_ptr<class ActorComponent> ActorComponentPtr;

typedef std::shared_ptr<class World> WorldPtr;
typedef std::weak_ptr<class World> WorldWeakPtr;

class ActorComponent : public std::enable_shared_from_this<ActorComponent>
{
public:
    virtual void addedToActor(const ActorPtr &actor);
    virtual void registerInWorld(const WorldPtr &world);

protected:
    ActorWeakPtr owner;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_ACTOR_COMPONENT_HPP