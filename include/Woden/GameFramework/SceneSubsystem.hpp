#ifndef WODEN_GAME_FRAMEWORK_SCENE_SUBSYSTEM_HPP
#define WODEN_GAME_FRAMEWORK_SCENE_SUBSYSTEM_HPP

#include "Subsystem.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace GameFramework
{

typedef std::shared_ptr<class ActorCameraComponent> ActorCameraComponentPtr;

/**
 * I am a subsystem that deals with scene presentations
 */
class SceneSubsystem : public Subsystem 
{
public:
    SceneSubsystem();
    ~SceneSubsystem();
    
    void addCameraComponent(const ActorCameraComponentPtr &component);
    void removeCameraComponent(const ActorCameraComponentPtr &component);
    ActorCameraComponentPtr findActiveCameraComponent();

    SceneGraph::ScenePtr scene;
    std::vector<ActorCameraComponentPtr> cameraComponents;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_SCENE_SUBSYSTEM_HPP
