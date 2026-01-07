#include "Woden/Physics/ContactManifold.hpp"

namespace Woden
{
namespace Physics
{

void ContactManifoldCache::beginEpoch()
{
    manifolds.clear();
    manifoldsMap.clear();
}

void ContactManifoldCache::endEpoch()
{
}

void ContactManifoldCache::addContactPoints(const std::vector<ContactPoint> &contacts, const CollisionObjectPtr &first, const CollisionObjectPtr &second)
{
    auto manifold = std::make_shared<ContactManifold> ();
    manifold->firstObject = first;
    manifold->secondObject = second;
    manifold->contacts = contacts;
    for(auto &contact : manifold->contacts)
    {
        contact.firstObject = first;
        contact.secondObject = second;
    }
    manifolds.push_back(manifold);
    manifoldsMap.insert(std::make_pair(std::make_pair(first, second), manifold));
}

Math::Vector3 ContactManifoldCache::getLastSeparatingAxis(const CollisionObjectPtr &first, const CollisionObjectPtr &second)
{
    (void)first;
    (void)second;
    // TODO: implement this properly.
    return Math::Vector3(1, 0, 0);
}

} // End of namespace Physics
} // End of namespace Woden
