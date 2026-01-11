#include "Woden/GameFramework/PlayerActor.hpp"

namespace Woden
{
namespace GameFramework
{

void PlayerActor::setupComponents()
{
    CharacterActor::setupComponents();

    cameraComponent = std::make_shared<ActorCameraComponent> ();
    cameraComponent->isActive = true;
    addComponent(cameraComponent);
}

} // End of namespace GameFramework
} // End of namespace Woden