#include "Woden/GameFramework/SceneSubsystem.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace GameFramework
{
SceneSubsystem::SceneSubsystem()
{
    scene = SceneGraph::MakeScene();
}

SceneSubsystem::~SceneSubsystem()
{
}

void SceneSubsystem::addCameraComponent(const ActorCameraComponentPtr &component)
{
    cameraComponents.push_back(component);
}

void SceneSubsystem::removeCameraComponent(const ActorCameraComponentPtr &component)
{
    for(size_t i = 0; i < cameraComponents.size(); ++i)
    {
        if(cameraComponents[i] == component)
        {
            cameraComponents.erase(cameraComponents.begin() + i);
            return;
        }
    }
}

ActorCameraComponentPtr SceneSubsystem::findActiveCameraComponent()
{
    for(auto &component : cameraComponents)
    {
        if(component->isActive)
            return component;
    }
    return nullptr;
}

} // End of namespace GameFramework
} // End of namespace Woden