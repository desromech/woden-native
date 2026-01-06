#include "Woden/Physics/PhysicsWorld.hpp"
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
    for(auto &object : collisionObjects)
        object->resetNetForces();

    for(auto &object : collisionObjects)
        object->integrateMovement(delta);
}
} // End of namespace Physics
} // End of namespace Woden