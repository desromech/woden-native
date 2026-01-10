#ifndef WODEN_PHYSICS_PHYSICS_WORLD_HPP
#define WODEN_PHYSICS_PHYSICS_WORLD_HPP

#include "ContactManifold.hpp"
#include "Woden/Math/Vector3.hpp"
#include <functional>
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
typedef std::shared_ptr<class ForceGenerator> ForceGeneratorPtr;

/**
 * I am an object that participates in colliosions.
 */
class PhysicsWorld : public std::enable_shared_from_this<PhysicsWorld>
{
public:
    void addCollisionObject(const CollisionObjectPtr &collisionObject);
    void addForceGenerator(const ForceGeneratorPtr &forceGenerator);

    virtual void update(Math::Scalar delta, Math::Scalar fixedTimeStep = 1.0f/120.0f);

    SceneGraph::ScenePtr buildInteractiveScene();

    Math::Vector3 gravity = Math::Vector3(0, -9.8f, 0);


    template<typename FT>
    void worldSpaceContactPointsDo(FT &&aBlock)
    {
        for(auto &manifold : contactManifoldCache.manifolds)
        {
            for(auto &contact : manifold->contacts)
                aBlock(contact.firstPoint, contact.secondPoint);
        }
    }

    void worldAnchorPointsDo(const std::function<void (Math::Vector3)> &aBlock);

protected:
    std::vector<CollisionObjectPtr> collisionObjects;
    std::vector<ForceGeneratorPtr> forceGenerators;
    ContactManifoldCache contactManifoldCache;
};

class DiscreteDynamicsPhysicsWorld : public PhysicsWorld
{
public:
    virtual void update(Math::Scalar delta, Math::Scalar fixedTimeStep = 1.0f/120.0f) override;

    Math::Scalar restingContactVelocityLimit = 0.1f;

protected:
    Math::Scalar accumulatedTime = 0;

    void resetNetForces();
    void evaluateForceGenerators(Math::Scalar delta);
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