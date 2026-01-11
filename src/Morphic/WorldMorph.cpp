#include "Woden/Morphic/WorldMorph.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"
#include "Woden/GameFramework/World.hpp"
#include "Woden/GameFramework/SceneSubsystem.hpp"
#include "Woden/Rendering/Camera.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"
#include "Woden/Rendering/SceneRenderer.hpp"

namespace Woden
{
namespace Morphic
{

WorldMorph::WorldMorph()
{
    color = Vector4(0, 0, 0, 1);
    setExtent(Vector2(1024, 576));

    cameraNode = std::make_shared<SceneGraph::SceneNode> ();
    auto cameraState = std::make_shared<Rendering::Camera> ();
    cameraNode->addCamera(cameraState);
    cameraNode->transform.translation = Math::Vector3(0, 0, 0);

    sceneRenderer = std::make_shared<Rendering::SceneRenderer> ();
}

void WorldMorph::update(Math::Scalar deltaTime)
{
    Morph::update(deltaTime);

    if(world)
    {
        // Update the world
        world->updateWithDeltaTime(deltaTime);

        // Get the scene.
        auto sceneSubsystem = world->getSceneSubsystem();
        scene = sceneSubsystem->scene;
        auto activeCameraComponent = sceneSubsystem->findActiveCameraComponent();
        if(activeCameraComponent)
            sceneCameraNode = activeCameraComponent->sceneNode;
    }
}

void WorldMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    Morph::drawWith(renderer);

    auto extent = getExtent();
    sceneRenderer->setupWithScreenSize(int(extent.x + 0.5), int(extent.y + 0.5));

    sceneRenderer->renderScene(renderer->renderingCommandList, scene, sceneCameraNode ? sceneCameraNode : cameraNode);
    auto guiTextureBinding = sceneRenderer->screen->getValidGuiTextureBinding();
    {
        Rendering::GuiElement screenQuad = {};
        screenQuad.type = Rendering::GuiElementType::TexturedRectangle;
        screenQuad.rectangleMax = extent;
        screenQuad.sourceImageRectangleMax = Vector2(1.0, 1.0);
        renderer->addGuiElementWithBinding(screenQuad, guiTextureBinding);
    }
}


} // End of namespace Morphic
} // End of namespace Woden