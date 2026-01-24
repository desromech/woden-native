#ifndef WODEN_TOOLS_LEVEL_EDITOR_SCENE_VIEW_MORPH_HPP
#define WODEN_TOOLS_LEVEL_EDITOR_SCENE_VIEW_MORPH_HPP

#include "Woden/Morphic/Morph.hpp"
#include "Woden/Morphic/SceneMorph.hpp"
#include "LevelEditorSceneViewMorph.hpp"

namespace Woden
{
namespace LevelEditor
{
using namespace Woden::Morphic;

typedef std::shared_ptr<class LevelEditorSceneViewMorph> LevelEditorSceneViewMorphPtr;

class LevelEditorSceneViewMorph : public SceneMorph
{
public:
    LevelEditorSceneViewMorph();

    virtual void handleMouseMotionEvent(const MouseMotionEventPtr &event) override;
    virtual void handleMouseWheelEvent(const MouseWheelEventPtr &event) override;
    virtual void handleMouseClickEvent(const MouseClickEventPtr &event) override;
    virtual void handleMouseDoubleClickEvent(const MouseDoubleClickEventPtr &event) override;

};

} // End of namespace LevelEditor
} // End of namespace Woden

#endif //WODEN_TOOLS_LEVEL_EDITOR_SCENE_VIEW_MORPH_HPP