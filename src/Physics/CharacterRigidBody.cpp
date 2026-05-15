#include "Woden/Physics/CharacterRigidBody.hpp"
#include <stdio.h>

namespace Woden
{
namespace Physics
{
void CharacterRigidBody::integrateMovement(Math::Scalar deltaTime)
{
    RigidBody::integrateMovement(deltaTime);
}

} // End of namespace Physics
} // End of namespace Woden