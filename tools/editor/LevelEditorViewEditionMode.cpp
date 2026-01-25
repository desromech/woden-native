#include "LevelEditorViewEditionMode.hpp"
#include "LevelEditorSceneViewMorph.hpp"
#include "Woden/Rendering/Camera.hpp"

namespace Woden
{
namespace LevelEditor
{

void LevelEditorViewEditionMode::onKeyboardDownEvent(const KeyboardDownEventPtr &event)
{
    auto sceneView = sceneViewReference.lock();

    switch(event->keySymbol)
    {
    case SDLK_KP_7:
        sceneView->cameraAngles = Vector3(-M_PI_2, 0, 0);
        sceneView->cameraNode->transform.rotation = sceneView->computeCameraOrientation();
        break;
    case SDLK_KP_1:
        sceneView->cameraAngles = Vector3(0);
        sceneView->cameraNode->transform.rotation = sceneView->computeCameraOrientation();
        break;
    case SDLK_KP_3:
        sceneView->cameraAngles = Vector3(0, -M_PI_2, 0);
        sceneView->cameraNode->transform.rotation = sceneView->computeCameraOrientation();
        break;
    case SDLK_KP_5:
    {
        auto &camera = sceneView->cameraNode->cameras.back();
        camera->isPerspective = !camera->isPerspective;
    }
        break;

    case SDLK_KP_PLUS:
    {
        auto &camera = sceneView->cameraNode->cameras.back();
        if(!camera->isPerspective)
            camera->focalDistance /= 1.1;
    }
        break;
    case SDLK_KP_MINUS:
    {
        auto &camera = sceneView->cameraNode->cameras.back();
        if(!camera->isPerspective)
            camera->focalDistance *= 1.1;
    }
        break;
    }
}

void LevelEditorViewEditionMode::onMouseMotionEvent(const MouseMotionEventPtr &event)
{
    auto sceneView = sceneViewReference.lock();
    if(event->hasLeftButtonDown())
    {
        sceneView->cameraAngles += Vector3(-event->delta.y, -event->delta.x, 0)*float(M_PI/180.0);
        sceneView->cameraNode->transform.rotation = sceneView->computeCameraOrientation();
        //printf("Scene morph left  %f,%f\n", event->delta.x, event->delta.y);
    }

    if(event->hasRightButtonDown())
    {
        sceneView->cameraNode->transform.translation +=
            sceneView->cameraNode->transform.rotation.rotateVector(
                Vector3(event->delta.x, -event->delta.y, 0).normalized()*0.05f
            );
    }
}

void LevelEditorViewEditionMode::onMouseWheelEvent(const MouseWheelEventPtr &event)
{
    auto sceneView = sceneViewReference.lock();

    auto &camera = sceneView->cameraNode->cameras.back();
    if(camera->isPerspective)
    {
        sceneView->cameraNode->transform.translation += 
        sceneView->cameraNode->transform.rotation.rotateVector(
            Vector3(event->scrollAmount.x, 0, -event->scrollAmount.y)*0.1f
        );
    }
    else
    {
        if(event->scrollAmount.y > 0)
            camera->focalDistance /= 1.1;
        else if (event->scrollAmount.y < 0)
            camera->focalDistance *= 1.1;
    }

}

} // End of namespace LevelEditor
} // End of namespace Woden