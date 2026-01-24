#include "LevelEditorSceneViewMorph.hpp"

namespace Woden
{
namespace LevelEditor
{
LevelEditorSceneViewMorph::LevelEditorSceneViewMorph()
{
}

void LevelEditorSceneViewMorph::handleKeyboardDownEvent(const KeyboardDownEventPtr &event)
{

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
