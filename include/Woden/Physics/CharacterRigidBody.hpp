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

    Math::Scalar stepHeight = 0.3f;
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CHARACTER_RIGID_BODY_HPP