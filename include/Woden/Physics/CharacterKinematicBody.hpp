#ifndef WODEN_PHYSICS_CHARACTER_KINEMATIC_BODY_HPP
#define WODEN_PHYSICS_CHARACTER_KINEMATIC_BODY_HPP

#include "GhostObject.hpp"

namespace Woden
{
namespace Physics
{
class CharacterKinematicBody : public GhostObject
{
    virtual void integrateMovement(Math::Scalar deltaTime) override;

    void stepUpMovement();
    void integrateBodyMovement(Math::Scalar deltaTime);
    void stepDownMovement();

    virtual bool isKinematicObject() const override
    {
        return true;
    }

    virtual bool isOnFloor() override
    {
        return isCurrentlyOnFloor;
    }

    bool isCurrentlyOnFloor = false;
    Math::Vector3 stepUpVector = Math::Vector3(0, 1, 0);
    Math::Scalar stepHeight = 0.3f;
    Math::Scalar stepUpOffset = 0.01f;

    Math::Scalar linearDamping = 0.2f;
    Math::Scalar angularDamping = 0.2f;
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CHARACTER_KINEMATIC_BODY_HPP