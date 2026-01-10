#ifndef WODEN_GAME_FRAMEWORK_ACTOR_SCENE_COMPONENT_HPP
#define WODEN_GAME_FRAMEWORK_ACTOR_SCENE_COMPONENT_HPP

#include "ActorComponent.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/Math/TRSTransform3D.hpp"
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

    Math::Vector3 getPosition() const;
    void setPosition(const Math::Vector3 &newPosition);

    Math::Quaternion getOrientation() const;
    void setOrientation(const Math::Quaternion &newOrientation);

    Math::TRSTransform3D getTransform() const;
    void setTransform(const Math::TRSTransform3D &newTransform);

    std::vector<ActorSceneComponentPtr> children;
    ActorSceneComponentWeakPtr parent;
    SceneGraph::SceneNodePtr sceneNode;

protected:
    void transformChanged();
    Math::TRSTransform3D transform;
};

class ActorMeshSceneComponent : public ActorSceneComponent
{
public:
    virtual void registerInWorld(const WorldPtr &world) override;

    Rendering::RenderablePtr mesh;

};

class ActorLightSourceComponent : public ActorSceneComponent
{
public:
    virtual void registerInWorld(const WorldPtr &world) override;
    
    Rendering::LightSourcePtr light;

};


} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_ACTOR_SCENE_COMPONENT_HPP