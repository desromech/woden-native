#ifndef WODEN_PHYSICS_ROPE_CONSTRAINT_HPP
#define WODEN_PHYSICS_ROPE_CONSTRAINT_HPP

#include "ObjectAnchoredPairUnionConstraint.hpp"

namespace Woden
{
namespace Physics
{

/**
 * I am a force generator used for modeling constraints.
 */
class RopeConstraint : public ObjectAnchoredPairUnionConstraint
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

    virtual void evaluateWithDeltaTime(Math::Scalar deltaTime) override
    {
        (void)deltaTime;

        auto firstAnchorPoint = computeFirstObjectAnchorPoint();
        auto secondAnchorPoint = computeSecondObjectAnchorPoint();

        auto ropeVector = firstAnchorPoint - secondAnchorPoint;
        auto ropeVectorLength = ropeVector.length();
        if(ropeVectorLength <= length)
            return;
        
        auto deltaLength = ropeVectorLength - length;
        auto forceDirection = ropeVector / ropeVectorLength;
        auto force = -forceDirection * Math::Vector3(stiffness*deltaLength);

        if(firstObject)
            firstObject->applyForceInPosition(force, firstAnchorPoint);

        if(secondObject)
            secondObject->applyForceInPosition(-force, secondAnchorPoint);
    }

    Math::Scalar length = 1;

};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_OBJECT_PAIR_UNION_CONSTRAINT_HPP
