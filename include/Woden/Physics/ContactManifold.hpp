#ifndef WODEN_PHYSICS_CONTACT_MANIFOLD_HPP
#define WODEN_PHYSICS_CONTACT_MANIFOLD_HPP

#include "ContactPoint.hpp"
#include "Woden/Math/Vector3.hpp"
#include <memory>
#include <vector>
#include <map>

namespace Woden
{
namespace Physics
{
typedef std::shared_ptr<struct ContactManifold> ContactManifoldPtr;

/**
 * I am a contact manifold. I keep track of a set of contact points
 */
struct ContactManifold
{
    CollisionObjectPtr firstObject;
    CollisionObjectPtr secondObject;

    std::vector<ContactPoint> contacts;

};

/**
 * I am a cache for contact manifolds.
 */
class ContactManifoldCache
{
public:
    void beginEpoch();
    void endEpoch();
    void addContactPoints(const std::vector<ContactPoint> &contacts, const CollisionObjectPtr &first, const CollisionObjectPtr &second);

    Math::Vector3 getLastSeparatingAxis(const CollisionObjectPtr &first, const CollisionObjectPtr &second);

    std::vector<ContactManifoldPtr> manifolds;
    std::map<std::pair<CollisionObjectPtr, CollisionObjectPtr>, ContactManifoldPtr> manifoldsMap;
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CONTACT_MANIFOLD_HPP