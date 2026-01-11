#ifndef WODEN_GAME_FRAMEWORK_PLAYER_ACTOR_HPP
#define WODEN_GAME_FRAMEWORK_PLAYER_ACTOR_HPP

#include "CharacterActor.hpp"

namespace Woden
{
namespace GameFramework
{

class PlayerActor : public CharacterActor
{
public:
    virtual void setupComponents() override;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_PLAYER_ACTOR_HPP