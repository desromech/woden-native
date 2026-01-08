#ifndef WODEN_PHYSICS_PHYSICS_WORLD_HPP
#define WODEN_PHYSICS_PHYSICS_WORLD_HPP

#include "ContactManifold.hpp"
#include "Woden/Math/Vector3.hpp"
#include <memory>
#include <vector>

namespace Woden
{
namespace SceneGraph
{
typedef std::shared_ptr<class Scene> ScenePtr;
}

namespace Physics
{
typedef std::shared_ptr<class CollisionObject> CollisionObjectPtr;
typedef std::weak_ptr<class PhysicsWorld> PhysicsWorldWeakPtr;

/**
 * I am an object that participates in colliosions.
 */
class PhysicsWorld : public std::enable_shared_from_this<PhysicsWorld>
{
public:
    void addCollisionObject(const CollisionObjectPtr &collisionObject);

    virtual void update(Math::Scalar delta);

    SceneGraph::ScenePtr buildInteractiveScene();

    Math::Vector3 gravity = Math::Vector3(0, -9.8, 0);

protected:
    std::vector<CollisionObjectPtr> collisionObjects;
    ContactManifoldCache contactManifoldCache;
};

class DiscreteDynamicsPhysicsWorld : public PhysicsWorld
{
public:
    virtual void update(Math::Scalar delta) override;

protected:
    void resetNetForces();
    void integrateMovement(Math::Scalar delta);
    
    void detectAndResolveCollisions();
    
    std::vector<std::pair<CollisionObjectPtr, CollisionObjectPtr>> computeBroadphaseCandidatePairs();
    void computeNarrowPhase(const std::vector<std::pair<CollisionObjectPtr, CollisionObjectPtr>> &broadphaseCandidates);
    void detectNarrowPhaseCollisionOf(const CollisionObjectPtr &first, const CollisionObjectPtr &second);
    void resolveContactManifoldsCollisionsAndConstraints();
    void solveCollisionContactResponseList(std::vector<ContactPoint> &contactList);
    void solveCollisionContactConstraintList(std::vector<ContactPoint> &contactList);
    void resolveContactCollisionResponse(ContactPoint &contact);
    void resolveContactConstraint(ContactPoint &contact, Math::Scalar relaxationFactor);
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_PHYSICS_WORLD_HPP