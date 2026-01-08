#ifndef WODEN_PHYSICS_CONTACT_POINT_HPP
#define WODEN_PHYSICS_CONTACT_POINT_HPP

#include "Woden/Math/Vector3.hpp"
#include "Woden/Math/RigidTransform.hpp"
#include <memory>

namespace Woden
{
namespace Physics
{

typedef std::shared_ptr<class CollisionObject> CollisionObjectPtr;

/**
 * I am a single contact point.
 */
struct ContactPoint
{
    Math::Vector3 normal;
    Math::Scalar requiredSeparation = 0;

    Math::Vector3 firstPoint;
    Math::Vector3 secondPoint;
    Math::Scalar penetrationDistance = 0;

    Math::Vector3 localFirstPoint;
    Math::Vector3 localSecondPoint;
    Math::Vector3 localFirstNormal;
    Math::Vector3 localSecondNormal;

    int32_t epoch = 0;
    CollisionObjectPtr firstObject;
    CollisionObjectPtr secondObject;

    Math::Scalar inverseInertia() const;
    Math::Scalar inverseLinearInertia() const;

    bool firstHasCollisionResponse() const;
    bool secondHasCollisionResponse() const;

    bool hasCollisionResponse() const
    {
        return firstHasCollisionResponse() || secondHasCollisionResponse();
    }

    bool isSimilarTo(const ContactPoint &o) const
    {
        return
            Math::closeTo(localFirstPoint, o.localFirstPoint) && 
            Math::closeTo(localSecondPoint, o.localSecondPoint);
    }

    Math::Vector3 getRelativeFirstPoint() const;
    Math::Vector3 getRelativeSecondPoint() const;

    void computeLocalVersionWithTransforms(const Math::RigidTransform &firstTransform, const Math::RigidTransform &secondTransform);
    void computeWorldVersionsWithTransforms(const Math::RigidTransform &firstTransform, const Math::RigidTransform &secondTransform);

    void update();

    void computeWorldContactPointAndDistances()
    {
        penetrationDistance = requiredSeparation + (secondPoint - firstPoint).dot(normal);
    }

    void flip()
    {
        normal = -normal;
        std::swap(firstPoint, secondPoint);
        std::swap(localFirstPoint, localSecondPoint);
        std::swap(localFirstNormal, localSecondNormal);
        std::swap(firstObject, secondObject);
    }
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CONTACT_POINT_HPP