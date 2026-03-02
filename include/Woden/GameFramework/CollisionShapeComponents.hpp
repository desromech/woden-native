#ifndef WODEN_GAME_FRAMEWORK_COLLISION_SHAPE_COMPONENT_HPP
#define WODEN_GAME_FRAMEWORK_COLLISION_SHAPE_COMPONENT_HPP

#include "ActorComponent.hpp"
#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Math/RigidTransform.hpp"

namespace Woden
{
namespace GameFramework
{

typedef std::shared_ptr<class AbstractCollisionShapeComponent> AbstractCollisionShapeComponentPtr;

/**
 * I am an abstract collision shape.
 */
class AbstractCollisionShapeComponent : public ActorComponent
{
public:
    virtual bool isCompoundCollisionShapeComponent() const;

    virtual void addedToActor(const ActorPtr &actor) override;

    virtual Woden::Physics::CollisionShapePtr asValidCollisionShapeWithoutTransform() = 0;
    virtual Woden::Physics::CollisionShapePtr asValidCollisionShape();

    Math::Scalar margin = 0.01f;
    Math::RigidTransform transform;
};

/**
 * I am a box collision shape.
 */
class BoxCollisionShapeComponent : public AbstractCollisionShapeComponent
{
public:
    Math::Vector3 halfExtent = Math::Vector3::Ones();

    virtual Woden::Physics::CollisionShapePtr asValidCollisionShapeWithoutTransform() override;
};

/**
 * I am a capsule collision shape.
 */
class AbstractCapsuleCollisionShapeComponent : public AbstractCollisionShapeComponent
{
public:
    Math::Scalar height = 1;
    Math::Scalar radius = 1;
};

/**
 * I am a capsule collision shape.
 */
class CapsuleYCollisionShapeComponent : public AbstractCapsuleCollisionShapeComponent
{
public:
    virtual Woden::Physics::CollisionShapePtr asValidCollisionShapeWithoutTransform() override;
};

/**
 * I am a compound collision shape.
 */
class CompoundCollisionShapeComponent : public AbstractCollisionShapeComponent
{
public:
    virtual Woden::Physics::CollisionShapePtr asValidCollisionShapeWithoutTransform() override;
    virtual Woden::Physics::CollisionShapePtr asValidCollisionShape() override;
    Woden::Physics::CollisionShapePtr asValidCollisionShapeWithTransform(const Math::RigidTransform &shapeTransform);

    std::vector<AbstractCollisionShapeComponentPtr> children;
};

} // End of namespace GameFramework
} // End of namespace Woden

#endif //WODEN_GAME_FRAMEWORK_COLLISION_SHAPE_COMPONENT_HPP