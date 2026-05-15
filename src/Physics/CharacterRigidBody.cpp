#include "Woden/Physics/CharacterRigidBody.hpp"
#include <stdio.h>

namespace Woden
{
namespace Physics
{
void CharacterRigidBody::integrateMovement(Math::Scalar deltaTime)
{
    RigidBody::integrateMovement(deltaTime);

    printf("Character constant manifolds %zu\n", contactManifolds.size());
}

} // End of namespace Physics
} // End of namespace Woden