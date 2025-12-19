#ifndef WODEN_PHYSICS_COLLISION_SHAPE
#define WODEN_PHYSICS_COLLISION_SHAPE

#include "Woden/Math/AABox.hpp"
#include "Woden/Math/Ray3D.hpp"
#include <memory>
#include <optional>

namespace Woden
{
namespace Physics
{

typedef std::shared_ptr<class CollisionShape> CollisionShapePtr;
typedef std::shared_ptr<class ConvexCollisionShape> ConvexCollisionShapePtr;
typedef std::shared_ptr<class BoxCollisionShape> BoxCollisionShapePtr;
typedef std::shared_ptr<class SphereCollisionShape> SphereCollisionShapePtr;

struct ShapeRayCastingResult
{
    CollisionShapePtr shape;
    float distance;
    Math::Vector3 point;
    Math::Vector3 normal;
};

// Collision shape
class CollisionShape
{
public:
    virtual bool isConvex() const;

    virtual std::optional<ShapeRayCastingResult> rayCast(const Math::Ray3D &ray);
};

// Convex collision shape
class ConvexCollisionShape : public CollisionShape
{
public:
    virtual bool isConvex() const override;

};

// Sphere collision shape
class SphereCollisionShape : public ConvexCollisionShape
{
public:
    Math::Scalar getRadius() const
    {
        return radius;
    }

    void setRadius(Math::Scalar newRadius)
    {
        radius = newRadius;
    }

    virtual std::optional<ShapeRayCastingResult> rayCast(const Math::Ray3D &ray);

protected:
    Math::Scalar radius = 1;
};

// AAbox collision shape
class BoxCollisionShape : public ConvexCollisionShape
{
public:
    Math::Vector3 getHalfExtent() const
    {
        return halfExtent;
    }

    void setHalfExtent(Math::Vector3 newHalfExtent)
    {
        halfExtent = newHalfExtent;
    }

    virtual std::optional<ShapeRayCastingResult> rayCast(const Math::Ray3D &ray);

protected:
    Math::Vector3 halfExtent = Math::Vector3(1, 1, 1);
};

} // End of namespace Physics
} // End of namespace Woden
#endif //WODEN_PHYSICS_COLLISION_SHAPE
