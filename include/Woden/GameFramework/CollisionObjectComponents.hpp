#ifndef WODEN_GAME_FRAMEWORK_COLLISION_OBJECT_COMPONENTS_HPP
#define WODEN_GAME_FRAMEWORK_COLLISION_OBJECT_COMPONENTS_HPP

#include "ActorComponent.hpp"
#include "Woden/Physics/CollisionObject.hpp"
#include "Woden/Math/TRSTransform3D.hpp"

namespace Woden
{
namespace GameFramework
{
typedef std::shared_ptr<class CollisionObjectComponent> CollisionObjectComponentPtr;

/**
 * I am a collision object component.
 */
class CollisionObjectComponent : public ActorComponent, public Physics::CollisionObjectModel
{
public:
    virtual void addedToActor(const ActorPtr &actor) override;
    virtual void registerInWorld(const WorldPtr &world) override;

    virtual Math::Scalar getMass() const
    {
        return 0.0f;
    }

    // Model interface
    virtual void loadCollisionStateInto(const Physics::CollisionObjectPtr &collisionObject) override;
    virtual void saveCollisionStateFrom(const Physics::CollisionObjectPtr &collisionObject) override;

    Math::Scalar restitutionCoefficient = 0.2f;
	Math::Scalar dynamicFrictionCoefficient = 0.5f;
	Math::Scalar staticFrictionCoefficient = 0.6f;

    Physics::CollisionObjectPtr collisionObject;

protected:
    void setupCollisionObject(const Physics::CollisionObjectPtr &collisionObject);
    virtual Physics::CollisionObjectPtr makeCollisionObjectInstance();

};

/**
 * I am a rigid body physical object.
 */
class RigidBodyComponent : public CollisionObjectComponent
{
public:
    virtual Math::Scalar getMass() const
    {
        return mass;
    }

    void setMass(Math::Scalar newMass)
    {
        mass = newMass;
    }

    void withoutTorque()
    {
        noTorque = true;
    }

protected:
    virtual Physics::CollisionObjectPtr makeCollisionObjectInstance() override;

    Math::Scalar mass = 0.0f;
    bool noTorque = false;
};

/**
 * I am a rigid body physical object.
 */
class CharacterBodyObjectComponent : public RigidBodyComponent
{
public:
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif // WODEN_GAME_FRAMEWORK_COLLISION_OBJECT_COMPONENTS_HPP
