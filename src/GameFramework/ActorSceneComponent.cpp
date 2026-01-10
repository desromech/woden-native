#include "Woden/GameFramework/ActorSceneComponent.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/SceneSubsystem.hpp"
#include "Woden/GameFramework/World.hpp"
#include <assert.h>

namespace Woden
{
namespace GameFramework
{

ActorSceneComponent::ActorSceneComponent()
{
    sceneNode = std::make_shared<SceneGraph::SceneNode> ();
}

ActorSceneComponent::~ActorSceneComponent()
{

}

void ActorSceneComponent::addChildSceneComponent(const ActorSceneComponentPtr &childComponent)
{
    children.push_back(childComponent);
    childComponent->addedToParentComponent(std::static_pointer_cast<ActorSceneComponent> (shared_from_this()));
}

void ActorSceneComponent::addedToActor(const ActorPtr &actor)
{
    ActorComponent::addedToActor(actor);
    auto self = std::static_pointer_cast<ActorSceneComponent> (shared_from_this());
    if(actor->rootSceneComponent)
        actor->rootSceneComponent->addChildSceneComponent(self);
    else
        actor->rootSceneComponent = self;
}

void ActorSceneComponent::addedToParentComponent(const ActorSceneComponentPtr &parentComponent)
{
    assert(!parent.lock());
    parent = parentComponent;
}

void ActorSceneComponent::registerInWorld(const WorldPtr &world)
{
    ActorComponent::registerInWorld(world);
    
    auto &sceneSubsystem = world->getSceneSubsystem();
    sceneNode = std::make_shared<SceneGraph::SceneNode> ();

    auto parentComponent = parent.lock();
    if(parentComponent)
    {
        parentComponent->sceneNode->addChild(sceneNode);
    }
    else
    {
        sceneSubsystem->scene->normalLayer->addChild(sceneNode);
    }
}

void ActorMeshSceneComponent::registerInWorld(const WorldPtr &world)
{
    ActorSceneComponent::registerInWorld(world);
    if(mesh)
        sceneNode->addRenderable(mesh);
}

} // End of namespace GameFramework
} // End of namespace Woden