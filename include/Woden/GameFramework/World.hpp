#ifndef WODEN_GAME_FRAMEWORK_WORLD_HPP
#define WODEN_GAME_FRAMEWORK_WORLD_HPP

#include <memory>
#include <vector>

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class WorldMorph> WorldMorphPtr;
typedef std::shared_ptr<class SystemWindow> SystemWindowPtr;
}

namespace GameFramework
{
typedef std::shared_ptr<class Actor> ActorPtr;
typedef std::shared_ptr<class World> WorldPtr;

/**
 * I am a world in the game framework
 */
class World : public std::enable_shared_from_this<World>
{
public:

    void spawnActor(const ActorPtr &actor);

    Morphic::WorldMorphPtr playInMorphic();
    Morphic::SystemWindowPtr playInSystemWindow();

protected:
    std::vector<ActorPtr> actors;
};

}
}
#endif //WODEN_GAME_FRAMEWORK_WORLD_HPP
