#ifndef WODEN_PHYSICS_CHARACTER_RIGID_BODY_HPP
#define WODEN_PHYSICS_CHARACTER_RIGID_BODY_HPP

#include "RigidBody.hpp"

namespace Woden
{
namespace Physics
{
class CharacterRigidBody : public RigidBody
{
    virtual void integrateMovement(Math::Scalar deltaTime) override;
    void stepUpMovement();
    void stepDownMovement();

    virtual bool isOnFloor() override
    {
        return isCurrentlyOnFloor;
    }

    bool isCurrentlyOnFloor = false;
    Math::Vector3 stepUpVector = Math::Vector3(0, 1, 0);
    Math::Scalar stepHeight = 0.3f;
    Math::Scalar stepUpOffset = 0.01f;
    
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CHARACTER_RIGID_BODY_HPP