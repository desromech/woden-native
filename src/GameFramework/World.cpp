#include "Woden/GameFramework/World.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/SceneSubsystem.hpp"
#include "Woden/Morphic/WorldMorph.hpp"

namespace Woden
{
namespace GameFramework
{

void World::beginPlay()
{
    if(isPlaying)
        return;
    
    isPlaying = true;
    for(auto &subsystem : subsystems)
        subsystem->beginPlay();
    for(auto &actor : actors)
        actor->beginPlay();
}

void World::spawnActor(const ActorPtr &actor)
{
    assert(!actor->world.lock());
    actor->world = shared_from_this();
    actors.push_back(actor);

    actor->registerWithSubsystemsInWorld(shared_from_this());
    if(isPlaying)
        actor->beginPlay();
}

Morphic::WorldMorphPtr World::playInMorphic()
{
    beginPlay();
    auto worldMorph = std::make_shared<Morphic::WorldMorph> ();
    worldMorph->world = shared_from_this();
    return worldMorph;
}

Morphic::SystemWindowPtr World::playInSystemWindow()
{
    auto worldMorph = playInMorphic();
    return worldMorph->openInSystemWindow();
}

void World::addSubsystem(const SubsystemPtr &subsystem)
{
    subsystems.push_back(subsystem);
    
    if(isPlaying)
        sceneSubsystem->beginPlay();
}

const SceneSubsystemPtr &World::getSceneSubsystem()
{
    if(!sceneSubsystem)
    {
        sceneSubsystem = std::make_shared<SceneSubsystem> ();
        addSubsystem(sceneSubsystem);
    }
    return sceneSubsystem;
}

} // End of namespace GameFramework
} // End of namespace Woden