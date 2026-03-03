#include "Woden/GameFramework/CollisionShapeComponents.hpp"
#include "Woden/GameFramework/Actor.hpp"

namespace Woden
{
namespace GameFramework
{

bool AbstractCollisionShapeComponent::isCompoundCollisionShapeComponent() const
{
    return false;
}

void AbstractCollisionShapeComponent::addedToActor(const ActorPtr &actor)
{
    ActorComponent::addedToActor(actor);

    auto self = std::static_pointer_cast<AbstractCollisionShapeComponent> (shared_from_this());
    if(actor->collisionShapeComponent)
    {
        fprintf(stderr, "TODO: support multiple collision shape components\n");
    }
    else
    {
        actor->collisionShapeComponent = self;
    }
}

Woden::Physics::CollisionShapePtr AbstractCollisionShapeComponent::asValidCollisionShape()
{
    // TODO: Add support for the transforms.
    return asValidCollisionShapeWithoutTransform();
}

Woden::Physics::CollisionShapePtr BoxCollisionShapeComponent::asValidCollisionShapeWithoutTransform()
{
    auto shape = std::make_shared<Physics::BoxCollisionShape> ();
    shape->margin = margin;
    shape->setHalfExtent(halfExtent);
    return shape;
}

Woden::Physics::CollisionShapePtr ConvexHullCollisionShapeComponent::asValidCollisionShapeWithoutTransform()
{
    auto shape = std::make_shared<Physics::ConvexHullCollisionShape> ();
    shape->margin = margin;
    shape->setCorners(corners);
    return shape;
}

Woden::Physics::CollisionShapePtr CapsuleYCollisionShapeComponent::asValidCollisionShapeWithoutTransform()
{
    auto shape = std::make_shared<Physics::CapsuleYCollisionShape> ();
    shape->margin = margin;
    shape->setHeightAndRadius(height, radius);
    return shape;
}

Woden::Physics::CollisionShapePtr CompoundCollisionShapeComponent::asValidCollisionShapeWithoutTransform()
{
    return asValidCollisionShapeWithTransform(Math::RigidTransform());
}

Woden::Physics::CollisionShapePtr CompoundCollisionShapeComponent::asValidCollisionShape()
{
    return asValidCollisionShapeWithTransform(transform);
}

Woden::Physics::CollisionShapePtr CompoundCollisionShapeComponent::asValidCollisionShapeWithTransform(const Math::RigidTransform &shapeTransform)
{
    auto compoundShape = std::make_shared<Woden::Physics::CompoundCollisionShape> ();
    for (auto &child : children)
    {
        auto childShape = child->asValidCollisionShapeWithoutTransform();
        if(!childShape->isConvex())
            continue;

        auto childTransform = shapeTransform.transformTransform(child->transform);
        compoundShape->addChild(std::static_pointer_cast<Physics::ConvexCollisionShape> (childShape), childTransform);
    }
    compoundShape->finishAddingChildren();
    return compoundShape;
}

} // End of namespace GameFramework
} // End of namespace Woden
