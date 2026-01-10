#include "Woden/GameFramework/ActorSceneComponents.hpp"
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
    sceneNode->transform = transform;

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

Math::Vector3 ActorSceneComponent::getPosition() const
{
    return transform.translation;
}

void ActorSceneComponent::setPosition(const Math::Vector3 &newPosition)
{
    transform.translation = newPosition;
    transformChanged();
}

Math::Quaternion ActorSceneComponent::getOrientation() const
{
    return transform.rotation;
}

void ActorSceneComponent::setOrientation(const Math::Quaternion &newOrientation)
{
    transform.rotation = newOrientation;
    transformChanged();
}

Math::TRSTransform3D ActorSceneComponent::getTransform() const
{
    return transform;
}

void ActorSceneComponent::setTransform(const Math::TRSTransform3D &newTransform)
{
    transform = newTransform;
    transformChanged();
}

void ActorSceneComponent::transformChanged()
{
    if(sceneNode)
        sceneNode->transform = transform;
} 

void ActorMeshSceneComponent::registerInWorld(const WorldPtr &world)
{
    ActorSceneComponent::registerInWorld(world);
    if(mesh)
        sceneNode->addRenderable(mesh);
}

void ActorLightSourceComponent::registerInWorld(const WorldPtr &world)
{
    ActorSceneComponent::registerInWorld(world);
    if(light)
        sceneNode->addLightSource(light);
}

} // End of namespace GameFramework
} // End of namespace Woden