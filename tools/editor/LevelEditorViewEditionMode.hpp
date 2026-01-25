#ifndef WODEN_EDITOR_LEVEl_VIEW_EDITION_MODE_HPP
#define WODEN_EDITOR_LEVEl_VIEW_EDITION_MODE_HPP

#include "LevelEditorViewMode.hpp"

namespace Woden
{
namespace LevelEditor
{

class LevelEditorViewEditionMode : public LevelEditorViewMode
{
public:
    virtual void onKeyboardDownEvent(const KeyboardDownEventPtr &event) override;

    virtual void onMouseMotionEvent(const MouseMotionEventPtr &event) override;
    virtual void onMouseWheelEvent(const MouseWheelEventPtr &event) override;
};

class LevelEditorViewObjectEditionMode : public LevelEditorViewEditionMode
{
public:    
};

} // End of namespace LevelEditor
} // End of namespace Woden
#endif //WODEN_EDITOR_LEVEl_VIEW_EDITION_MODE_HPP