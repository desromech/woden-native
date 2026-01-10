#ifndef WODEN_GAME_FRAMEWORK_PHYSICS_SUBSYSTEM_HPP
#define WODEN_GAME_FRAMEWORK_PHYSICS_SUBSYSTEM_HPP

#include "Subsystem.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"
#include <vector>

namespace Woden
{
namespace GameFramework
{

/**
 * I am a subsystem that deals with dispatching the tick event.
 */
class PhysicsSubsystem : public Subsystem 
{
public:
    PhysicsSubsystem();
    ~PhysicsSubsystem();

    virtual void updateSingleTimeStep(Math::Scalar deltaTime) override;
    
    Physics::PhysicsWorldPtr physicsWorld;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_PHYSICS_SUBSYSTEM_HPP
