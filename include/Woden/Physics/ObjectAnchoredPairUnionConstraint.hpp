#ifndef WODEN_PHYSICS_OBJECT_ANCHORED_PAIR_UNION_CONSTRAINT_HPP
#define WODEN_PHYSICS_OBJECT_ANCHORED_PAIR_UNION_CONSTRAINT_HPP

#include "ObjectPairUnionConstraint.hpp"
#include "CollisionObject.hpp"
#include "Woden/Math/Vector3.hpp"

namespace Woden
{
namespace Physics
{

/**
 * I am a force generator used for modeling constraints.
 */
class ObjectAnchoredPairUnionConstraint : public ObjectPairUnionConstraint
{
public:
    Math::Vector3 computeFirstObjectAnchorPoint() const
    {
        if(!firstObject)
            return firstObjectLocalAnchorPoint;
        return firstObject->getTransform().transformPosition(firstObjectLocalAnchorPoint);
    }

    Math::Vector3 computeSecondObjectAnchorPoint() const
    {
        if(!secondObject)
            return secondObjectLocalAnchorPoint;
        return secondObject->getTransform().transformPosition(secondObjectLocalAnchorPoint);
    }

    Math::Vector3 firstObjectLocalAnchorPoint = Math::Vector3::Zeros();
    Math::Vector3 secondObjectLocalAnchorPoint = Math::Vector3::Zeros();
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_OBJECT_PAIR_UNION_CONSTRAINT_HPP
