#include "Woden/Physics/ContactManifold.hpp"
#include "Woden/Physics/CollisionObject.hpp"

namespace Woden
{
namespace Physics
{

void ContactManifold::addContactPoint(const ContactPoint &contactPoint)
{
    auto point = contactPoint;
    point.firstObject = firstObject;
    point.secondObject = secondObject;

    for(size_t i = 0; i < contacts.size(); ++i)
    {
        if(contacts[i].isSimilarTo(point))
        {
            contacts[i] = point;
            return;
        }
    }

    contacts.push_back(point);
}

void ContactManifold::expireContactsUntil(int32_t expiredEpoch)
{
    size_t destIndex = 0;
    for(size_t i = 0; i < contacts.size(); ++i)
    {
        auto &contact = contacts[i];
        if(contact.epoch >= expiredEpoch)
            contacts[destIndex++] = contact;
    }

    contacts.resize(destIndex);
}

bool ContactManifold::firstHasCollisionResponse() const
{
    return int(firstObject->collisionResponseGroupMask) & int(secondObject->collisionGroup);
}

bool ContactManifold::secondHasCollisionResponse() const
{
    return int(secondObject->collisionResponseGroupMask) & int(firstObject->collisionGroup);
}

void ContactManifoldCache::beginEpoch()
{
    epoch = epoch + 1;
    expireOldContacts();
    //manifolds.clear();
    //manifoldsMap.clear();
}

void ContactManifoldCache::endEpoch()
{
    expireOldManifolds();
}

void ContactManifoldCache::expireOldContacts()
{
    for(auto &manifold : manifolds)
        manifold->expireContactsUntil(getExpiredEpoch());

}

void ContactManifoldCache::expireOldManifolds()
{

}

ContactManifoldPtr ContactManifoldCache::getOrCreateManifoldFor(const CollisionObjectPtr &first, const CollisionObjectPtr &second)
{
    auto it = manifoldsMap.find(std::make_pair(first, second));
    if(it != manifoldsMap.end())
        return it->second;

    auto manifold = std::make_shared<ContactManifold> ();
    manifold->firstObject = first;
    manifold->secondObject = second;
    manifolds.push_back(manifold);
    manifoldsMap.insert(std::make_pair(std::make_pair(first, second), manifold));
    return manifold;
}

void ContactManifoldCache::addContactPoints(const std::vector<ContactPoint> &contacts, const CollisionObjectPtr &first, const CollisionObjectPtr &second)
{
    auto manifold = getOrCreateManifoldFor(first, second);
    for(auto contact : contacts)
    {
        contact.epoch = epoch;
        manifold->addContactPoint(contact);
    }

    //printf("Manifold contacts %zu\n", manifold->contacts.size());
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
