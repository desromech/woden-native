#ifndef WODEN_GAME_FRAMEWORK_CHARACTER_ACTOR_HPP
#define WODEN_GAME_FRAMEWORK_CHARACTER_ACTOR_HPP

#include "Actor.hpp"

namespace Woden
{
namespace GameFramework
{

class CharacterActor : public Actor
{
public:
    CharacterActor();
    ~CharacterActor();

    virtual Math::Scalar getDefaultHeight() const
    {
        return 1.6f;
    }

    virtual Math::Scalar getDefaultBroadness() const
    {
        return 0.5f;
    }

    virtual Math::Scalar getDefaultMass() const
    {
        return 70.0f;
    }

    virtual Math::Vector3 getDefaultJumpVelocity() const
    {
        return Math::Vector3(0, 2, 0);
    }

    virtual void setupComponents() override;

    virtual void tick(Math::Scalar delta) override;

    Math::Vector3 walkingDirection = Math::Vector3::Zeros();
    Math::Scalar walkingSpeed = 3.0f;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_CHARACTER_ACTOR_HPP