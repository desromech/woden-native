#include "Woden/GameFramework/PlayerActor.hpp"
#include "Woden/GameFramework/InputComponents.hpp"

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
    addComponent(std::make_shared<InputListenerComponent> ());
}

void PlayerActor::tick(Math::Scalar delta)
{
    CharacterActor::tick(delta);
    
    cameraAngles.x = Math::clamp(cameraAngles.x, Math::Scalar(-M_PI_2), Math::Scalar(M_PI_2));
    cameraComponent->setOrientation(Math::Quaternion::XRotation(cameraAngles.x));
    setOrientation(Math::Quaternion::YRotation(cameraAngles.y));
}

void PlayerActor::handleKeyboardDownEvent(const KeyboardDownEventPtr &event)
{
    printf("PlayerActor::handleKeyboardDownEvent\n");
}

void PlayerActor::handleKeyboardUpEvent(const KeyboardUpEventPtr &event)
{
    printf("PlayerActor::handleKeyboardUpEvent\n");
}


void PlayerActor::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    if(event->hasLeftButtonDown())
        cameraAngles += Math::Vector3(-event->delta.y, -event->delta.x, 0)*Math::Vector3(M_PI/180.0);
}

} // End of namespace GameFramework
} // End of namespace Woden