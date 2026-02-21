#include "Woden/GameFramework/PhysicsSubsystem.hpp"
#include "Woden/GameFramework/SceneSubsystem.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"

namespace Woden
{
namespace GameFramework
{

PhysicsSubsystem::PhysicsSubsystem()
{
    physicsWorld = std::make_shared<Physics::DiscreteDynamicsPhysicsWorld> ();
}

PhysicsSubsystem::~PhysicsSubsystem()
{
}

void PhysicsSubsystem::updateSingleTimeStep(Math::Scalar deltaTime)
{
    physicsWorld->loadCollisionStateFromModels();
    physicsWorld->updateSingleTimeStep(deltaTime);
    physicsWorld->saveCollisionStateIntoModels();
    if(debugEnabled)
        updateDebugContacts();
}

void PhysicsSubsystem::updateDebugContacts()
{
    if(!contactPointsNode)
    {
        contactPointsNode = std::make_shared<SceneGraph::SceneNode> ();
        sceneSubsystem->scene->normalLayer->addChild(contactPointsNode);

        auto contactMeshMaterial = std::make_shared<Rendering::MetallicRoughnessMaterial> ();
        contactMeshMaterial->baseColorFactor = Math::Vector4(0, 0, 0, 1);
        contactMeshMaterial->emissiveFactor = Math::Vector3(1, 0, 0);
        contactMeshMaterial->roughnessFactor = 1.0;
        contactMeshMaterial->metallicFactor = 0.0;

        contactPointMesh = Rendering::MeshBuilder()
                .setMaterial(contactMeshMaterial)
                .addCubeWithExtent(Math::Vector3(0.1f))
                .generateTexcoordsWithFacePlanarTransformWithScale(Math::Vector2(1, 1))
                .finishMesh();
    }
    
    contactPointsNode->removeAllChildren();

    physicsWorld->worldSpaceContactPointsDo([&](const Math::Vector3 &firstPoint, const Math::Vector3 &secondPoint) {
        // First contact point.
        {
            auto contactNode = std::make_shared<SceneGraph::SceneNode> ();
            contactNode->transform.translation = firstPoint;
            contactNode->addRenderable(contactPointMesh);
            contactPointsNode->addChild(contactNode);
        }

        // Second contact point.
        {
            auto contactNode = std::make_shared<SceneGraph::SceneNode> ();
            contactNode->transform.translation = secondPoint;
            contactNode->addRenderable(contactPointMesh);
            contactPointsNode->addChild(contactNode);
        }
    });

}

} // End of namespace GameFramework
} // End of namespace Woden
