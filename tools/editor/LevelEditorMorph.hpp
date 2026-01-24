#ifndef WODEN_TOOLS_LEVEL_EDITOR_MORPH_HPP
#define WODEN_TOOLS_LEVEL_EDITOR_MORPH_HPP

#include "Woden/Morphic/Morph.hpp"
#include "Woden/Morphic/MenuBarMorph.hpp"
#include "Woden/Morphic/ToolBarMorph.hpp"
#include "Woden/Morphic/StatusBarMorph.hpp"
#include "Woden/Morphic/SceneMorph.hpp"
#include "LevelEditorSceneViewMorph.hpp"
#include "LevelModel.hpp"

namespace Woden
{
namespace LevelEditor
{
using namespace Woden::Morphic;

class LevelEditorMorph : public Morph
{
public:
    LevelEditorMorph();
    
    virtual void initialize() override;

    void createMenuBar();

    MenuBarMorphPtr menuBar;
    ToolBarMorphPtr toolBar;
    LevelEditorSceneViewMorphPtr sceneView;
    StatusBarMorphPtr statusBar;
    LevelModelPtr model;
};

} // End of namespace LevelEditor
} // End of namespace Woden

#endif //WODEN_TOOLS_LEVEL_EDITOR_MORPH_HPP