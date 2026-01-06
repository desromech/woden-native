#ifndef WODEN_PHYSICS_RIGID_BODY_HPP
#define WODEN_PHYSICS_RIGID_BODY_HPP

#include "CollisionObject.hpp"
#include "Woden/Math/Matrix3x3.hpp"
namespace Woden
{
namespace Physics
{
/**
 * I am an object that participates in colliosions.
 */
class RigidBody : public CollisionObject
{
public:
    Math::Scalar mass = 0;
    Math::Matrix3x3 inertiaTensor = Math::Matrix3x3::Zeros();

    void computeMassDistribution();
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_RIGID_BODY_HPP