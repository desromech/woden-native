#ifndef WODEN_GAME_FRAMEWORK_PLAYER_ACTOR_HPP
#define WODEN_GAME_FRAMEWORK_PLAYER_ACTOR_HPP

#include "CharacterActor.hpp"
#include "ActorSceneComponents.hpp"

namespace Woden
{
namespace GameFramework
{

class PlayerActor : public CharacterActor
{
public:
    virtual void setupComponents() override;
    
    virtual void tick(Math::Scalar delta) override;

    virtual void handleKeyboardDownEvent(const KeyboardDownEventPtr &event) override;
    virtual void handleKeyboardUpEvent(const KeyboardUpEventPtr &event) override;

    virtual void handleMouseMotionEvent(const MouseMotionEventPtr &event) override;

    ActorCameraComponentPtr cameraComponent;

    Math::Vector3 cameraAngles = Math::Vector3::Zeros();
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_PLAYER_ACTOR_HPP