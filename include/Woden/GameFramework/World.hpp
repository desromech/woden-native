#ifndef WODEN_GAME_FRAMEWORK_WORLD_HPP
#define WODEN_GAME_FRAMEWORK_WORLD_HPP

#include "Woden/Math/Scalar.hpp"
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
typedef std::shared_ptr<class Subsystem> SubsystemPtr;
typedef std::shared_ptr<class ActorTickSubsystem> ActorTickSubsystemPtr;
typedef std::shared_ptr<class SceneSubsystem> SceneSubsystemPtr;
typedef std::shared_ptr<class World> WorldPtr;

/**
 * I am a world in the game framework
 */
class World : public std::enable_shared_from_this<World>
{
public:
    World();
    ~World();

    void beginPlay();

    void spawnActor(const ActorPtr &actor);

    Morphic::WorldMorphPtr playInMorphic();
    Morphic::SystemWindowPtr playInSystemWindow();

    void addSubsystem(const SubsystemPtr &subsystem);
    const ActorTickSubsystemPtr &getActorTickSubsystem();
    const SceneSubsystemPtr &getSceneSubsystem();

    void updateWithDeltaTime(Math::Scalar deltaTime);
    void updateSingleTimeStep(Math::Scalar deltaTime);

    Math::Scalar updateTimeStep = 1.0f / 60.0f;
    Math::Scalar currentTime = 0.0f;

protected:
    std::vector<ActorPtr> actors;
    std::vector<SubsystemPtr> subsystems;
    ActorTickSubsystemPtr actorTickSubsystem;
    SceneSubsystemPtr sceneSubsystem;

    bool isPlaying = false;
    Math::Scalar accumulatedDeltaTime = 0.0f;

};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_WORLD_HPP
