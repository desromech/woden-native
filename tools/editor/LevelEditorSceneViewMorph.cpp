#include "LevelEditorSceneViewMorph.hpp"
#include "LevelEditorMorph.hpp"
#include "Woden/Rendering/Camera.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace LevelEditor
{
LevelEditorSceneViewMorph::LevelEditorSceneViewMorph()
{
}

LevelEditorMorphPtr LevelEditorSceneViewMorph::getLevelEditor() const
{
    return std::static_pointer_cast<LevelEditorMorph> (owner.lock());
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

    case SDLK_KP_PLUS:
    {
        auto &camera = cameraNode->cameras.back();
        if(!camera->isPerspective)
            camera->focalDistance /= 1.1;
    }
        break;
    case SDLK_KP_MINUS:
    {
        auto &camera = cameraNode->cameras.back();
        if(!camera->isPerspective)
            camera->focalDistance *= 1.1;
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
    auto &camera = cameraNode->cameras.back();
    if(!camera->isPerspective)
    {
        if(event->scrollAmount.y > 0)
            camera->focalDistance /= 1.1;
        else if (event->scrollAmount.y < 0)
            camera->focalDistance *= 1.1;
        return;
    }
        
    SceneMorph::handleMouseWheelEvent(event);
}

Math::Ray3D LevelEditorSceneViewMorph::computeRayForScreenPosition(const Vector2 &screenPosition)
{
    auto extent = getExtent();
    auto aspect = extent.x / extent.y;
    auto frustum = cameraNode->cameras.front()->computeViewFrustum(aspect);
    auto worldFrustum = frustum.transformedWithMatrix(cameraNode->transform.asMatrix());
    //printf("screen pos %f %f\n", screenPosition.x, screenPosition.y);
    auto normalizedPoint = Vector2(screenPosition.x / extent.x, 1.0 - screenPosition.y / extent.y);

    //printf("NDC: %f %f\n", ndc.x, ndc.y);
    return worldFrustum.rayForNormalizedPoint(normalizedPoint);
}

void LevelEditorSceneViewMorph::handleMouseClickEvent(const MouseClickEventPtr &event)
{
    auto ray = computeRayForScreenPosition(event->position - getOrigin());
    getLevelEditor()->rayPick(ray);
}

void LevelEditorSceneViewMorph::handleMouseDoubleClickEvent(const MouseDoubleClickEventPtr &event)
{
    printf("TODO: Double click at %f %f\n", event->position.x, event->position.y);

}

} // End of namespace LevelEditor
} // End of namespace Woden
