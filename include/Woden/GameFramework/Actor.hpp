#ifndef WODEN_GAME_FRAMEWORK_ACTOR_HPP
#define WODEN_GAME_FRAMEWORK_ACTOR_HPP

#include <memory>

namespace Woden
{
namespace GameFramework
{
typedef std::shared_ptr<class Actor> ActorPtr;
typedef std::weak_ptr<class World> WorldWeakPtr;

class Actor
{
public:
    WorldWeakPtr owner;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_ACTOR_HPP