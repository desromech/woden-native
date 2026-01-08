#include "Woden/Physics/PhysicsWorld.hpp"
#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Physics/RigidBody.hpp"
#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
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

    auto contactPointsNode = std::make_shared<SceneGraph::SceneNode> ();
    scene->normalLayer->addChild(contactPointsNode);

    auto contactMeshMaterial = std::make_shared<Rendering::MetallicRoughnessMaterial> ();
    contactMeshMaterial->baseColorFactor = Math::Vector4(0, 0, 0, 1);
    contactMeshMaterial->emissiveFactor = Math::Vector3(1, 0, 0);
    contactMeshMaterial->roughnessFactor = 1.0;
    contactMeshMaterial->metallicFactor = 0.0;

    auto contactMesh = Rendering::MeshBuilder()
            .setMaterial(contactMeshMaterial)
            .addCubeWithExtent(Math::Vector3(0.1))
            .generateTexcoordsWithFacePlanarTransformWithScale(Math::Vector2(1, 1))
            .finishMesh();

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

        // Update the contact points.
        contactPointsNode->removeAllChildren();

        worldSpaceContactPointsDo([&](const Math::Vector3 &firstPoint, const Math::Vector3 &secondPoint) {
            // First contact point.
            {
                auto contactNode = std::make_shared<SceneGraph::SceneNode> ();
                contactNode->transform.translation = firstPoint;
                contactNode->addRenderable(contactMesh);
                contactPointsNode->addChild(contactNode);
            }

            // Second contact point.
            {
                auto contactNode = std::make_shared<SceneGraph::SceneNode> ();
                contactNode->transform.translation = secondPoint;
                contactNode->addRenderable(contactMesh);
                contactPointsNode->addChild(contactNode);
            }
        });
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
        if(!manifold->hasCollisionResponse())
            continue;;

        solveCollisionContactResponseList(manifold->contacts);
        solveCollisionContactConstraintList(manifold->contacts);
    }
}

void DiscreteDynamicsPhysicsWorld::solveCollisionContactResponseList(std::vector<ContactPoint> &contactList)
{
    for(auto &contact : contactList)
        resolveContactCollisionResponse(contact);

}

void DiscreteDynamicsPhysicsWorld::resolveContactCollisionResponse(ContactPoint &contact)
{
    if(!contact.hasCollisionResponse())
        return;
    contact.update();

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
    if(contact.firstHasCollisionResponse())
        firstCollisionObject->applyImpulse(impulsePerInverseMass);
    if(contact.secondHasCollisionResponse())
        secondCollisionObject->applyImpulse(impulsePerInverseMass);
}

void DiscreteDynamicsPhysicsWorld::solveCollisionContactConstraintList(std::vector<ContactPoint> &contactList)
{
    Math::Scalar relaxationFactor = 1;
    for(auto &contact : contactList)
        resolveContactConstraint(contact, relaxationFactor);
}

void DiscreteDynamicsPhysicsWorld::resolveContactConstraint(ContactPoint &contact, Math::Scalar relaxationFactor)
{
    if(!contact.hasCollisionResponse())
        return;

    contact.update();
    auto penetrationDistance = contact.penetrationDistance;
    if(penetrationDistance < 0)
        return;
    
    auto &firstCollisionObject = contact.firstObject;
    auto &secondCollisionObject = contact.secondObject;

    auto inverseInertia = contact.inverseInertia();
    if(inverseInertia <= 0)
        return;
    
    auto penetrationDelta = penetrationDistance*relaxationFactor/inverseInertia;
    if(contact.firstHasCollisionResponse())
        firstCollisionObject->applyMovementAtRelativePoint(penetrationDelta, contact.getRelativeFirstPoint(), contact.normal);
    if(contact.secondHasCollisionResponse())
        secondCollisionObject->applyMovementAtRelativePoint(penetrationDelta, contact.getRelativeSecondPoint(), -contact.normal);
}

} // End of namespace Physics
} // End of namespace Woden