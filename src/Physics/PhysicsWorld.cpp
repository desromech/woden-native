#include "Woden/Physics/PhysicsWorld.hpp"
#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Physics/RigidBody.hpp"
#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include <assert.h>
#include <unordered_map>

namespace Woden
{
namespace Physics
{

void PhysicsWorld::addCollisionObject(const CollisionObjectPtr &collisionObject)
{
    assert(!collisionObject->owner.lock());
    
    collisionObject->owner = shared_from_this();
    collisionObjects.push_back(collisionObject);
}

void PhysicsWorld::update(Math::Scalar delta)
{
    (void)delta;
}

SceneGraph::ScenePtr PhysicsWorld::buildInteractiveScene()
{
    auto scene = SceneGraph::MakeScene();
    {
        auto directionalLightSource = std::make_shared<Woden::Rendering::DirectionalLightSource> ();
        directionalLightSource->color = Math::Vector3(0.8, 0.8, 0.7);
        directionalLightSource->castShadows = true;

        auto lightNode = directionalLightSource->asSceneNode();
        lightNode->transform.rotation = Math::Quaternion::YRotationDegrees(-135)*Math::Quaternion::XRotationDegrees(-45);
        lightNode->transform.translation = Math::Vector3(-1.0, 2.0, -2.0);

        scene->normalLayer->addChild(lightNode);
    }

    std::unordered_map<CollisionObjectPtr, SceneGraph::SceneNodePtr> collisionObjectsToSceneNodeMap;
    for(auto &collisionObject : collisionObjects)
    {
        auto sceneNode = collisionObject->constructVisualizationSceneNode();
        if(sceneNode)
        {
            scene->normalLayer->addChild(sceneNode);
            collisionObjectsToSceneNodeMap.insert(std::make_pair(collisionObject, sceneNode));
        }
    }

    auto world = shared_from_this();
    scene->updateFunction = [=](Math::Scalar deltaTime) {
        // Update the world.
        world->update(deltaTime);

        // Update the scene nodes.
        for(auto &pair : collisionObjectsToSceneNodeMap)
        {
            auto collisionObject = pair.first;
            auto sceneNode = pair.second;
            sceneNode->transform = collisionObject->getTransform().asTRSTransform3D();
        }
    };

    return scene;
}

void DiscreteDynamicsPhysicsWorld::update(Math::Scalar delta)
{
    resetNetForces();
    integrateMovement(delta);
    detectAndResolveCollisions();
}


void DiscreteDynamicsPhysicsWorld::resetNetForces()
{
    for(auto &object : collisionObjects)
        object->resetNetForces();
}

void DiscreteDynamicsPhysicsWorld::integrateMovement(Math::Scalar delta)
{
    for(auto &object : collisionObjects)
        object->integrateMovement(delta);
}

void DiscreteDynamicsPhysicsWorld::detectAndResolveCollisions()
{
    auto candidatePairs = computeBroadphaseCandidatePairs();
    computeNarrowPhase(candidatePairs);
    resolveContactManifoldsCollisionsAndConstraints();
}

std::vector<std::pair<CollisionObjectPtr, CollisionObjectPtr>> DiscreteDynamicsPhysicsWorld::computeBroadphaseCandidatePairs()
{
    std::vector<std::pair<CollisionObjectPtr, CollisionObjectPtr>> candidatePairs;
    for(size_t i = 0; i < collisionObjects.size(); ++i)
    {
        auto &firstCollisionObject = collisionObjects[i];
        auto firstBoundingBox = firstCollisionObject->getWorldBoundingBoxWithMargin();
        for(size_t j = i + 1; j < collisionObjects.size(); ++j)
        {
            auto &secondCollisionObject = collisionObjects[j];
            auto secondBoundingBox = secondCollisionObject->getWorldBoundingBoxWithMargin();

            // Filter objects that do not need collision detection.
            if(!firstCollisionObject->needsCollisionDetection() && !secondCollisionObject->needsCollisionDetection())
                continue;

            if(firstBoundingBox.hasIntersectionWithBox(secondBoundingBox))
                candidatePairs.push_back(std::make_pair(firstCollisionObject, secondCollisionObject));
        }

    }

    return candidatePairs;
}

void DiscreteDynamicsPhysicsWorld::computeNarrowPhase(const std::vector<std::pair<CollisionObjectPtr, CollisionObjectPtr>> &broadphaseCandidates)
{
    contactManifoldCache.beginEpoch();
    for(auto &pair : broadphaseCandidates)
        detectNarrowPhaseCollisionOf(pair.first, pair.second);
    contactManifoldCache.endEpoch();
}

void DiscreteDynamicsPhysicsWorld::detectNarrowPhaseCollisionOf(const CollisionObjectPtr &first, const CollisionObjectPtr &second)
{
    auto firstShape = first->shape;
    auto firstTransform = first->getTransform();

    auto secondShape = second->shape;
    auto secondTransform = second->getTransform();

    Math::Vector3 separatingAxisHint = contactManifoldCache.getLastSeparatingAxis(first, second);

    std::vector<ContactPoint> contactPoints = firstShape->detectAndComputeCollisionContactPoints(firstTransform, secondShape, secondTransform, separatingAxisHint);
    if(!contactPoints.empty())
        contactManifoldCache.addContactPoints(contactPoints, first, second);
}

void DiscreteDynamicsPhysicsWorld::resolveContactManifoldsCollisionsAndConstraints()
{
    for(auto &manifold : contactManifoldCache.manifolds)
    {
        solveCollisionContactResponseList(manifold->contacts);
        solveCollisionContactConstraintList(manifold->contacts);
    }
}

void DiscreteDynamicsPhysicsWorld::solveCollisionContactResponseList(const std::vector<ContactPoint> &contactList)
{
    for(auto &contact : contactList)
        resolveContactCollisionResponse(contact);

}

void DiscreteDynamicsPhysicsWorld::resolveContactCollisionResponse(const ContactPoint &contact)
{
	auto firstCollisionObject = contact.firstObject;
	auto secondCollisionObject = contact.secondObject;
	
	auto contactNormal = contact.normal;

    auto firstContactVelocity = firstCollisionObject->linearVelocity;
	auto secondContactVelocity = secondCollisionObject->linearVelocity;

    auto relativeVelocity = firstContactVelocity - secondContactVelocity;
    auto separatingSpeed = relativeVelocity.dot(contactNormal);
    if(separatingSpeed >= 0)
        return;

    auto restitutionCoefficient = Math::sqrt(firstCollisionObject->restitutionCoefficient*secondCollisionObject->restitutionCoefficient);
    auto newSeparatingSpeed = -separatingSpeed*restitutionCoefficient;
    auto deltaVelocity = newSeparatingSpeed - separatingSpeed;

    auto totalInverseMass = firstCollisionObject->getInverseMass() + secondCollisionObject->getInverseMass();
    if(totalInverseMass <= 0)
        return;

    auto impulse = deltaVelocity / totalInverseMass;
    auto impulsePerInverseMass = contactNormal*impulse;
    firstCollisionObject->applyImpulse(impulsePerInverseMass);
    secondCollisionObject->applyImpulse(impulsePerInverseMass);
}

void DiscreteDynamicsPhysicsWorld::solveCollisionContactConstraintList(const std::vector<ContactPoint> &contactList)
{
    Math::Scalar relaxationFactor = 1;
    for(auto &contact : contactList)
        resolveContactConstraint(contact, relaxationFactor);
}

void DiscreteDynamicsPhysicsWorld::resolveContactConstraint(const ContactPoint &contact, Math::Scalar relaxationFactor)
{
    auto penetrationDistance = contact.penetrationDistance;
    if(penetrationDistance < 0)
        return;
    
    auto &firstCollisionObject = contact.firstObject;
    auto &secondCollisionObject = contact.secondObject;

    auto inverseInertia = contact.inverseLinearInertia();
    if(inverseInertia <= 0)
        return;
    
    auto penetrationDelta = penetrationDistance*relaxationFactor/inverseInertia;
    firstCollisionObject->applyMovementAtRelativePoint(penetrationDelta, contact.getRelativeFirstPoint(), contact.normal);
    secondCollisionObject->applyMovementAtRelativePoint(penetrationDelta, contact.getRelativeSecondPoint(), -contact.normal);
}

} // End of namespace Physics
} // End of namespace Woden