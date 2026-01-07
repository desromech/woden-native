#ifndef WODEN_PHYSICS_CONTACT_MANIFOLD_HPP
#define WODEN_PHYSICS_CONTACT_MANIFOLD_HPP

#include "ContactPoint.hpp"
#include "Woden/Math/Vector3.hpp"
#include <memory>

namespace Woden
{
namespace Physics
{
typedef std::shared_ptr<class CollisionObject> CollisionObjectPtr;

/**
 * I am a contact manifold. I keep track of a set of contact points
 */
struct ContactManifold
{
};

/**
 * I am a cache for contact manifolds.
 */
class ContactManifoldCache
{
public:
    Math::Vector3 getLastSeparatingAxis(const CollisionObjectPtr &first, const CollisionObjectPtr &second);
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CONTACT_MANIFOLD_HPP