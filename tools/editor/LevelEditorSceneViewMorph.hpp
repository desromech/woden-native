#ifndef WODEN_TOOLS_LEVEL_EDITOR_SCENE_VIEW_MORPH_HPP
#define WODEN_TOOLS_LEVEL_EDITOR_SCENE_VIEW_MORPH_HPP

#include "Woden/Morphic/Morph.hpp"
#include "Woden/Morphic/SceneMorph.hpp"
#include "Woden/Math/Ray3D.hpp"
#include "LevelModel.hpp"

namespace Woden
{
namespace LevelEditor
{
using namespace Woden::Morphic;

typedef std::shared_ptr<class LevelEditorViewMode> LevelEditorViewModePtr;

typedef std::shared_ptr<class LevelEditorMorph> LevelEditorMorphPtr;
typedef std::shared_ptr<class LevelEditorSceneViewMorph> LevelEditorSceneViewMorphPtr;

class LevelEditorSceneViewMorph : public SceneMorph
{
public:
    LevelEditorSceneViewMorph();

    virtual void initialize() override;
    void setViewMode(const LevelEditorViewModePtr &newViewMode);

    LevelEditorMorphPtr getLevelEditor() const;

    virtual void onKeyboardDownEvent(const KeyboardDownEventPtr &event) override;
    virtual void onKeyboardUpEvent(const KeyboardUpEventPtr &event) override;
    virtual void onTextInputEvent(const TextInputEventPtr &event) override;

    virtual void onMouseButtonDownEvent(const MouseButtonDownEventPtr &event) override;
    virtual void onMouseButtonUpEvent(const MouseButtonUpEventPtr &event) override;
    virtual void onMouseMotionEvent(const MouseMotionEventPtr &event) override;
    virtual void onMouseWheelEvent(const MouseWheelEventPtr &event) override;
    virtual void onMouseClickEvent(const MouseClickEventPtr &event) override;
    virtual void onMouseDoubleClickEvent(const MouseDoubleClickEventPtr &event) override;

    Math::Ray3D computeRayForScreenPosition(const Vector2 &screenPosition);

protected:
    LevelEditorViewModePtr viewMode;
};

} // End of namespace LevelEditor
} // End of namespace Woden

#endif //WODEN_TOOLS_LEVEL_EDITOR_SCENE_VIEW_MORPH_HPP