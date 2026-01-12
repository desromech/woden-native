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

void PlayerActor::tick(Math::Scalar delta)
{
    CharacterActor::tick(delta);
    
    cameraAngles.x = Math::clamp(cameraAngles.x, Math::Scalar(-M_PI_2), Math::Scalar(M_PI_2));
    cameraComponent->setOrientation(Math::Quaternion::XRotation(cameraAngles.x));
    setOrientation(Math::Quaternion::YRotation(cameraAngles.y));
}

} // End of namespace GameFramework
} // End of namespace Woden