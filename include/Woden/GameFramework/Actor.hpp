#ifndef WODEN_GAME_FRAMEWORK_ACTOR_HPP
#define WODEN_GAME_FRAMEWORK_ACTOR_HPP

#include "Woden/Math/TRSTransform3D.hpp"
#include <memory>
#include <vector>

namespace Woden
{
namespace GameFramework
{
typedef std::shared_ptr<class Actor> ActorPtr;
typedef std::shared_ptr<class ActorComponent> ActorComponentPtr;
typedef std::shared_ptr<class ActorSceneComponent> ActorSceneComponentPtr;
typedef std::shared_ptr<class World> WorldPtr;
typedef std::weak_ptr<class World> WorldWeakPtr;

class Actor : public std::enable_shared_from_this<Actor>
{
public:
    void addComponent(const ActorComponentPtr &component);
    virtual void beginPlay();
    virtual void registerWithSubsystemsInWorld(const WorldPtr &world);

    Math::Vector3 getPosition() const;
    void setPosition(const Math::Vector3 &newPosition);

    Math::Quaternion getOrientation() const;
    void setOrientation(const Math::Quaternion &newOrientation);

    Math::TRSTransform3D getTransform() const;
    void setTransform(const Math::TRSTransform3D &newTransform);

    WorldWeakPtr world;
    std::vector<ActorComponentPtr> components;
    ActorSceneComponentPtr rootSceneComponent;
    bool isRegisteredInWorld = false;

};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_ACTOR_HPP