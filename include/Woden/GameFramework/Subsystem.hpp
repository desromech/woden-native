#ifndef WODEN_GAME_FRAMEWORK_SUBSYSTEM_HPP
#define WODEN_GAME_FRAMEWORK_SUBSYSTEM_HPP

#include "Woden/Math/Scalar.hpp"
#include <memory>

namespace Woden
{
namespace GameFramework
{

/**
 * I am a specific subsystem inside of the game framework
 */
class Subsystem
{
public:
    virtual ~Subsystem() {}
    virtual void beginPlay() {}

    virtual void prePhysicsUpdateSingleTimeStep(Math::Scalar deltaTime)
    {
        (void)deltaTime;
    }

    virtual void updateSingleTimeStep(Math::Scalar deltaTime)
    {
        (void)deltaTime;
    }

    virtual void postActorMessagesUpdateSingleTimeStep(Math::Scalar deltaTime)
    {
        (void)deltaTime;
    }
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_SUBSYSTEM_HPP
