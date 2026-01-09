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

void PhysicsWorld::update(Math::Scalar delta, Math::Scalar fixedTimeStep)
{
    (void)delta;
    (void)fixedTimeStep;
}

SceneGraph::ScenePtr PhysicsWorld::buildInteractiveScene()
{
    auto scene = SceneGraph::MakeScene();
    {
        auto directionalLightSource = std::make_shared<Woden::Rendering::DirectionalLightSource> ();
        directionalLightSource->color = Math::Vector3(0.8f, 0.8f, 0.7f);
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

void DiscreteDynamicsPhysicsWorld::update(Math::Scalar delta, Math::Scalar fixedTimeStep)
{
    accumulatedTime += delta;

    // Clamp to avoid spiral of death. See https://code.tutsplus.com/how-to-create-a-custom-2d-physics-engine-the-core-engine--gamedev-7493t
    if(accumulatedTime >= 0.2f)
        accumulatedTime = 0.2f;

    while(accumulatedTime >= fixedTimeStep)
    {
        accumulatedTime -= fixedTimeStep;

        resetNetForces();
        integrateMovement(fixedTimeStep);
        detectAndResolveCollisions();
    }
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
    const int IterationCount = 5; 
    for(int i = 0; i < IterationCount; ++i)
    {
        // Collision responses
        for(auto &manifold : contactManifoldCache.manifolds)
        {
            if(!manifold->hasCollisionResponse())
                continue;

            solveCollisionContactResponseList(manifold->contacts);
        }

        // Collision contraints
        for(auto &manifold : contactManifoldCache.manifolds)
        {
            if(!manifold->hasCollisionResponse())
                continue;

            solveCollisionContactConstraintList(manifold->contacts);
        }
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

	// See Milling. 'Game Physics Engine Development'. Chapter 14 for details on these equations and the associated algorithms.	
	auto firstCollisionObject = contact.firstObject;
	auto secondCollisionObject = contact.secondObject;
	
	auto contactNormal = contact.normal;

	auto relativeFirstPoint = contact.getRelativeFirstPoint();
	auto relativeSecondPoint = contact.getRelativeSecondPoint();

	auto contactLocalToWorldMatrix3x3 = contact.computeContactSpaceMatrix();

    auto velocityChangePerImpulseWorldMatrix = 
        firstCollisionObject->computeVelocityPerImpulseWorldMatrixForRelativeContactPoint(relativeFirstPoint) +
        secondCollisionObject->computeVelocityPerImpulseWorldMatrixForRelativeContactPoint(relativeSecondPoint);
	
	auto velocityChangePerImpulseContactMatrix = contactLocalToWorldMatrix3x3.transpose() * velocityChangePerImpulseWorldMatrix * contactLocalToWorldMatrix3x3;

    auto inverseMass = firstCollisionObject->getInverseMass() + secondCollisionObject->getInverseMass();
	velocityChangePerImpulseContactMatrix = velocityChangePerImpulseContactMatrix + (Math::Matrix3x3(inverseMass));
	
    if (velocityChangePerImpulseContactMatrix.determinant() == 0)
    {
        return;
    }

	auto impulseChangePerVelocityContactMatrix = velocityChangePerImpulseContactMatrix.inverse();

    auto firstContactVelocity = firstCollisionObject->computeVelocityAtRelativePoint(relativeFirstPoint);
	auto secondContactVelocity = secondCollisionObject->computeVelocityAtRelativePoint(relativeSecondPoint);
	
    auto relativeSeparationVelocity = firstContactVelocity - secondContactVelocity;
	
    auto relativeContactSeparationVelocity = relativeSeparationVelocity * contactLocalToWorldMatrix3x3;
	if(relativeContactSeparationVelocity.x > 0)
    {
        return;
    }

	auto relativeVelocityFromIntegrationDelta = firstCollisionObject->linearVelocityIntegrationDelta - secondCollisionObject->linearVelocityIntegrationDelta;
	auto relativeContactVelocityFromIntegrationDelta = relativeVelocityFromIntegrationDelta.dot(contactNormal);
	
	auto restitutionCoefficient = Math::sqrt(firstCollisionObject->restitutionCoefficient * secondCollisionObject->restitutionCoefficient);
	
	// Resting contact: reduce contact velocity by acceleration only speed increase, and set the restitution coefficient to 0.
	if(abs(relativeContactSeparationVelocity.x) < restingContactVelocityLimit)
    { 
		restitutionCoefficient = 0;
    }

	auto deltaVelocity = -relativeContactSeparationVelocity.x - (restitutionCoefficient * (relativeContactSeparationVelocity.x - relativeContactVelocityFromIntegrationDelta));
	
	auto contactLocalVelocityChange = Math::Vector3(
        deltaVelocity,
		-relativeContactSeparationVelocity.y,
		-relativeContactSeparationVelocity.z
    );
		
	auto contactLocalImpulse = impulseChangePerVelocityContactMatrix * contactLocalVelocityChange;

	// Compute the planar length for simulating friction.
	auto staticFrictionCoefficient = std::min(firstCollisionObject->staticFrictionCoefficient, secondCollisionObject->staticFrictionCoefficient);
	auto planarImpulse = Math::sqrt(contactLocalImpulse.y*contactLocalImpulse.y + contactLocalImpulse.z*contactLocalImpulse.z);

	// Is this in the limits for the static friction?
	if(planarImpulse > contactLocalImpulse.x * staticFrictionCoefficient)
    {
        auto dynamicFrictionCoefficient = std::min(firstCollisionObject->dynamicFrictionCoefficient, secondCollisionObject->dynamicFrictionCoefficient);
		contactLocalImpulse.y /= planarImpulse;
		contactLocalImpulse.z /= planarImpulse;

		//contactLocalImpulse yz length = dynamicFrictionCoefficient * contactLocalImpulse x
		
		// CHECK ME: What is the meaning of this correction? [From Millington Game Physics Engine Development, Chapter 15 pp 410]
		//auto frictionNormalDelta = velocityChangePerImpulseContactMatrix.firstRow().dot(Math::Vector3(1, dynamicFrictionCoefficient*contactLocalImpulse.y, dynamicFrictionCoefficient*contactLocalImpulse.z));
        //contactLocalImpulse.x = deltaVelocity / frictionNormalDelta;

		contactLocalImpulse.y *= dynamicFrictionCoefficient * contactLocalImpulse.x;
		contactLocalImpulse.z *= dynamicFrictionCoefficient * contactLocalImpulse.x;
	}

	auto contactImpulse = contactLocalToWorldMatrix3x3 * contactLocalImpulse;

	if(contact.firstHasCollisionResponse())
        firstCollisionObject->applyImpulseInRelativePosition(contactImpulse, relativeFirstPoint);

	if(contact.secondHasCollisionResponse())
        secondCollisionObject->applyImpulseInRelativePosition(-contactImpulse, relativeSecondPoint);
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