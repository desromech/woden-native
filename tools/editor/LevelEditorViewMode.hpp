#ifndef WODEN_EDITOR_LEVEl_VIEW_MODE_HPP
#define WODEN_EDITOR_LEVEl_VIEW_MODE_HPP

#include "Woden/Events/Events.hpp"

namespace Woden
{
namespace LevelEditor
{
using namespace Woden::Events;

typedef std::shared_ptr<class LevelEditorViewMode> LevelEditorViewModePtr;
typedef std::weak_ptr<class LevelEditorSceneViewMorph> LevelEditorSceneViewMorphWeakPtr;

class LevelEditorViewMode
{
public:    
    virtual void onKeyboardDownEvent(const KeyboardDownEventPtr &event);
    virtual void onKeyboardUpEvent(const KeyboardUpEventPtr &event);
    virtual void onTextInputEvent(const TextInputEventPtr &event);

    virtual void onMouseButtonDownEvent(const MouseButtonDownEventPtr &event);
    virtual void onMouseButtonUpEvent(const MouseButtonUpEventPtr &event);
    virtual void onMouseMotionEvent(const MouseMotionEventPtr &event);
    virtual void onMouseWheelEvent(const MouseWheelEventPtr &event);
    virtual void onMouseClickEvent(const MouseClickEventPtr &event);
    virtual void onMouseDoubleClickEvent(const MouseDoubleClickEventPtr &event);

    LevelEditorSceneViewMorphWeakPtr sceneViewReference;
};

} // End of namespace LevelEditor
} // End of namespace Woden

#endif //WODEN_EDITOR_LEVEl_VIEW_MODE_HPP