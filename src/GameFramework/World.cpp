#include "Woden/GameFramework/World.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/Morphic/WorldMorph.hpp"

namespace Woden
{
namespace GameFramework
{

void World::spawnActor(const ActorPtr &actor)
{
}

Morphic::WorldMorphPtr World::playInMorphic()
{
    auto worldMorph = std::make_shared<Morphic::WorldMorph> ();
    worldMorph->world = shared_from_this();
    return worldMorph;
}

Morphic::SystemWindowPtr World::playInSystemWindow()
{
    auto worldMorph = playInMorphic();
    return worldMorph->openInSystemWindow();
}

} // End of namespace GameFramework
} // End of namespace Woden