#ifndef WODEN_PHYSICS_ROD_CONSTRAINT_HPP
#define WODEN_PHYSICS_ROD_CONSTRAINT_HPP

#include "ObjectAnchoredPairUnionConstraint.hpp"

namespace Woden
{
namespace Physics
{

/**
 * I am a fixed rod constraint.
 */
class RodConstraint : public ObjectAnchoredPairUnionConstraint
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

        auto rodVector = firstAnchorPoint - secondAnchorPoint;
        auto rodVectorLength = rodVector.length();
        if(rodVectorLength <= 0.0)
            return;
        
        auto deltaLength = rodVectorLength - length;
        auto forceDirection = rodVector / rodVectorLength;
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

#endif //WODEN_PHYSICS_ROD_CONSTRAINT_HPP
