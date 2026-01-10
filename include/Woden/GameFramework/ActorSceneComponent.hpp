#ifndef WODEN_GAME_FRAMEWORK_ACTOR_SCENE_COMPONENT_HPP
#define WODEN_GAME_FRAMEWORK_ACTOR_SCENE_COMPONENT_HPP

#include "ActorComponent.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include <vector>

namespace Woden
{
namespace GameFramework
{
typedef std::shared_ptr<class ActorSceneComponent> ActorSceneComponentPtr;
typedef std::weak_ptr<class ActorSceneComponent> ActorSceneComponentWeakPtr;

class ActorSceneComponent : public ActorComponent
{
public:
    ActorSceneComponent();
    ~ActorSceneComponent();

    void addChildSceneComponent(const ActorSceneComponentPtr &childComponent);
    
    virtual void addedToActor(const ActorPtr &actor) override;
    void addedToParentComponent(const ActorSceneComponentPtr &parentComponent);

    virtual void registerInWorld(const WorldPtr &world) override;

    std::vector<ActorSceneComponentPtr> children;
    ActorSceneComponentWeakPtr parent;
    SceneGraph::SceneNodePtr sceneNode;
};

class ActorMeshSceneComponent : public ActorSceneComponent
{
public:
    virtual void registerInWorld(const WorldPtr &world) override;
    
    Rendering::RenderablePtr mesh;

};


} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_ACTOR_SCENE_COMPONENT_HPP