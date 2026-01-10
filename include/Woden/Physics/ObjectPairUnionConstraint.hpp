#ifndef WODEN_PHYSICS_OBJECT_PAIR_UNION_CONSTRAINT_HPP
#define WODEN_PHYSICS_OBJECT_PAIR_UNION_CONSTRAINT_HPP

#include "ForceGeneratorConstraint.hpp"

namespace Woden
{
namespace Physics
{

typedef std::shared_ptr<class CollisionObject> CollisionObjectPtr;

/**
 * I am a force generator used for modeling constraints.
 */
class ObjectPairUnionConstraint : public ForceGeneratorConstraint
{
public:
    Math::Scalar stiffness = 50.0;
	Math::Scalar damping = 10.0;
    CollisionObjectPtr firstObject;
    CollisionObjectPtr secondObject;
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_OBJECT_PAIR_UNION_CONSTRAINT_HPP
