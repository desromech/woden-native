#ifndef WODEN_GAME_FRAMEWORK_PHYSICS_SUBSYSTEM_HPP
#define WODEN_GAME_FRAMEWORK_PHYSICS_SUBSYSTEM_HPP

#include "Subsystem.hpp"
#include "Woden/Physics/PhysicsWorld.hpp"
#include "Woden/SceneGraph/Scene.hpp"
#include <vector>

namespace Woden
{
namespace GameFramework
{

typedef std::shared_ptr<class SceneSubsystem> SceneSubsystemPtr;

/**
 * I am a subsystem that deals with dispatching the tick event.
 */
class PhysicsSubsystem : public Subsystem 
{
public:
    PhysicsSubsystem();
    ~PhysicsSubsystem();

    virtual void updateSingleTimeStep(Math::Scalar deltaTime) override;
    void updateDebugContacts();
    
    Physics::PhysicsWorldPtr physicsWorld;
    SceneSubsystemPtr sceneSubsystem;

    // Contact point debugging.
    bool debugEnabled = true;
    SceneGraph::SceneNodePtr contactPointsNode;
    Rendering::RenderablePtr contactPointMesh;


};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_PHYSICS_SUBSYSTEM_HPP
