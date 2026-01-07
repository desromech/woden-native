#ifndef WODEN_PHYSICS_CONTACT_POINT_HPP
#define WODEN_PHYSICS_CONTACT_POINT_HPP

#include "Woden/Math/Vector3.hpp"
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
    
    CollisionObjectPtr firstObject;
    CollisionObjectPtr secondObject;

    void computeWorldContactPointAndDistances()
    {
        penetrationDistance = requiredSeparation + (secondPoint - firstPoint).dot(normal);
    }

    void flip()
    {
        normal = -normal;
        std::swap(firstPoint, secondPoint);
    }
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CONTACT_POINT_HPP