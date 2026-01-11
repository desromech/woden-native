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

    virtual void setupComponents() override;

    virtual void tick(Math::Scalar delta) override;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_CHARACTER_ACTOR_HPP