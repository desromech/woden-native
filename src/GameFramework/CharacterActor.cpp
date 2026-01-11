#include "Woden/GameFramework/CharacterActor.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"

namespace Woden
{
namespace GameFramework
{

CharacterActor::CharacterActor()
{
    setWantsToTick(true);
}

CharacterActor::~CharacterActor()
{
}

void CharacterActor::tick(Math::Scalar deltaTime)
{
    (void)deltaTime;
}

void CharacterActor::setupComponents()
{
    Actor::setupComponents();

    addComponent(std::make_shared<ActorSceneComponent> ());

}

} // End of namespace GameFramework
} // End of namespace Woden