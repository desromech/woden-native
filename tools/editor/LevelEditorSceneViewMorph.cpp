#include "LevelEditorSceneViewMorph.hpp"
#include "LevelEditorMorph.hpp"
#include "Woden/Rendering/Camera.hpp"
#include "Woden/SceneGraph/Scene.hpp"
#include "LevelEditorViewEditionMode.hpp"

namespace Woden
{
namespace LevelEditor
{
LevelEditorSceneViewMorph::LevelEditorSceneViewMorph()
{
}

void LevelEditorSceneViewMorph::initialize()
{
    SceneMorph::initialize();
    setViewMode(std::make_shared<LevelEditorViewObjectEditionMode> ());
}

void LevelEditorSceneViewMorph::setViewMode(const LevelEditorViewModePtr &newViewMode)
{
    newViewMode->sceneViewReference = std::static_pointer_cast<LevelEditorSceneViewMorph> (shared_from_this());
    viewMode = newViewMode;
}

LevelEditorMorphPtr LevelEditorSceneViewMorph::getLevelEditor() const
{
    return std::static_pointer_cast<LevelEditorMorph> (owner.lock());
}

void LevelEditorSceneViewMorph::onKeyboardDownEvent(const KeyboardDownEventPtr &event)
{
    if(viewMode)
        viewMode->onKeyboardDownEvent(event);
}

void LevelEditorSceneViewMorph::onKeyboardUpEvent(const KeyboardUpEventPtr &event)
{
    if(viewMode)
        viewMode->onKeyboardUpEvent(event);
}

void LevelEditorSceneViewMorph::onTextInputEvent(const TextInputEventPtr &event)
{
    if(viewMode)
        viewMode->onTextInputEvent(event);
}

void LevelEditorSceneViewMorph::onMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    if(viewMode)
        viewMode->onMouseButtonDownEvent(event);
}

void LevelEditorSceneViewMorph::onMouseButtonUpEvent(const MouseButtonUpEventPtr &event)
{
    if(viewMode)
        viewMode->onMouseButtonUpEvent(event);
}

void LevelEditorSceneViewMorph::onMouseMotionEvent(const MouseMotionEventPtr &event)
{
    takeKeyboardFocus();
    if(viewMode)
        viewMode->onMouseMotionEvent(event);
}

void LevelEditorSceneViewMorph::onMouseWheelEvent(const MouseWheelEventPtr &event)
{
    if(viewMode)
        viewMode->onMouseWheelEvent(event);
}

void LevelEditorSceneViewMorph::onMouseClickEvent(const MouseClickEventPtr &event)
{
    if(viewMode)
        viewMode->onMouseClickEvent(event);
}

void LevelEditorSceneViewMorph::onMouseDoubleClickEvent(const MouseDoubleClickEventPtr &event)
{
    if(viewMode)
        viewMode->onMouseDoubleClickEvent(event);
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

} // End of namespace LevelEditor
} // End of namespace Woden
