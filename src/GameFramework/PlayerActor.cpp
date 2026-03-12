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

    auto cameraTransform = Math::TRSTransform3D();
    cameraTransform.translation = Math::Vector3(0, getDefaultHeight()*0.5, 0);
    cameraComponent->setTransform(cameraTransform);
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
    if(event->isKeyRepeat)
        return;

    switch(event->keySymbol)
    {
    case SDLK_w:
        walkingDirection.z = -1;
        break;
    case SDLK_a:
        walkingDirection.x = -1;
        break;
    case SDLK_s:
        walkingDirection.z = 1;
        break;
    case SDLK_d:
        walkingDirection.x = 1;
        break;
    }

    //printf("walkingDirection: %f %f %f\n", walkingDirection.x, walkingDirection.y, walkingDirection.z);
}

void PlayerActor::handleKeyboardUpEvent(const KeyboardUpEventPtr &event)
{
    if(event->isKeyRepeat)
        return;

    switch(event->keySymbol)
    {
    case SDLK_w:
        if (walkingDirection.z < 0)
            walkingDirection.z = 0;
        break;
    case SDLK_a:
        if (walkingDirection.x < 0)
            walkingDirection.x = 0;
        break;
    case SDLK_s:
        if (walkingDirection.z > 0)
            walkingDirection.z = 0;
        break;
    case SDLK_d:
        if (walkingDirection.x > 0)
            walkingDirection.x = 0;
        break;
    }

    //printf("walkingDirection: %f %f %f\n", walkingDirection.x, walkingDirection.y, walkingDirection.z);
}

void PlayerActor::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    if(event->hasLeftButtonDown())
        cameraAngles += Math::Vector3(-event->delta.y, -event->delta.x, 0)*Math::Vector3(M_PI/180.0);
}

} // End of namespace GameFramework
} // End of namespace Woden