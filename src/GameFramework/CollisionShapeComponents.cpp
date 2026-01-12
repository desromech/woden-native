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

Woden::Physics::CollisionShapePtr CapsuleYCollisionShapeComponent::asValidCollisionShapeWithoutTransform()
{
    auto shape = std::make_shared<Physics::CapsuleYCollisionShape> ();
    shape->margin = margin;
    shape->setHeightAndRadius(height, radius);
    return shape;
}

} // End of namespace GameFramework
} // End of namespace Woden
