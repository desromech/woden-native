#include "LevelEditorSceneViewMorph.hpp"
#include "Woden/Rendering/Camera.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace LevelEditor
{
LevelEditorSceneViewMorph::LevelEditorSceneViewMorph()
{
}

void LevelEditorSceneViewMorph::handleKeyboardDownEvent(const KeyboardDownEventPtr &event)
{
    switch(event->keySymbol)
    {
    case SDLK_KP_7:
        cameraAngles = Vector3(-M_PI_2, 0, 0);
        cameraNode->transform.rotation = computeCameraOrientation();
        break;
    case SDLK_KP_1:
        cameraAngles = Vector3(0);
        cameraNode->transform.rotation = computeCameraOrientation();
        break;
    case SDLK_KP_3:
        cameraAngles = Vector3(0, -M_PI_2, 0);
        cameraNode->transform.rotation = computeCameraOrientation();
        break;
    case SDLK_KP_5:
    {
        auto &camera = cameraNode->cameras.back();
        camera->isPerspective = !camera->isPerspective;
    }
        break;
    }
}

void LevelEditorSceneViewMorph::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    SceneMorph::handleMouseMotionEvent(event);
    takeKeyboardFocus();
}

void LevelEditorSceneViewMorph::handleMouseWheelEvent(const MouseWheelEventPtr &event)
{
    SceneMorph::handleMouseWheelEvent(event);
}

void LevelEditorSceneViewMorph::handleMouseClickEvent(const MouseClickEventPtr &event)
{
    printf("TODO: Click at %f %f\n", event->position.x, event->position.y);
}

void LevelEditorSceneViewMorph::handleMouseDoubleClickEvent(const MouseDoubleClickEventPtr &event)
{
    printf("TODO: Double click at %f %f\n", event->position.x, event->position.y);

}

} // End of namespace LevelEditor
} // End of namespace Woden
