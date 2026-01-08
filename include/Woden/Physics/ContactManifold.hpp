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
    void addContactPoint(const ContactPoint &contactPoint);
    void expireContactsUntil(int32_t expiredEpoch);

    bool firstHasCollisionResponse() const;
    bool secondHasCollisionResponse() const;

    bool hasCollisionResponse() const
    {
        return firstHasCollisionResponse() || secondHasCollisionResponse();
    }

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
    static const int32_t oldestKeepEpoch = 4;

    void beginEpoch();
    void endEpoch();

    int32_t getExpiredEpoch() const
    {
        return epoch - oldestKeepEpoch;
    }

    void addContactPoints(const std::vector<ContactPoint> &contacts, const CollisionObjectPtr &first, const CollisionObjectPtr &second);

    Math::Vector3 getLastSeparatingAxis(const CollisionObjectPtr &first, const CollisionObjectPtr &second);
    ContactManifoldPtr getOrCreateManifoldFor(const CollisionObjectPtr &first, const CollisionObjectPtr &second);

    int32_t epoch = 0;
    std::vector<ContactManifoldPtr> manifolds;
    std::map<std::pair<CollisionObjectPtr, CollisionObjectPtr>, ContactManifoldPtr> manifoldsMap;
private:
    void expireOldContacts();
    void expireOldManifolds();
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CONTACT_MANIFOLD_HPP