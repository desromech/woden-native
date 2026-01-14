#include "LevelEditorMorph.hpp"
#include "Woden/Morphic/Layout.hpp"

namespace Woden
{
namespace LevelEditor
{

LevelEditorMorph::LevelEditorMorph()
{
    setExtent(Vector2(1024, 576));
}

bool LevelEditorMorph::initialize()
{
    menuBar = std::make_shared<MenuBarMorph> ();
    toolBar = std::make_shared<ToolBarMorph> ();
    sceneView = std::make_shared<Morph> ();
    statusBar = std::make_shared<StatusBarMorph> ();

    sceneView->color = Vector4(0, 1, 0, 1);

    addMorph(menuBar);
    addMorph(toolBar);
    addMorph(sceneView);
    addMorph(statusBar);

    auto layout = std::make_shared<VerticalPackingLayout> ();
    layout->addMorph(menuBar, 0, true);
    layout->addMorph(toolBar, 0, true);
    layout->addMorph(sceneView, 1, true);
    layout->addMorph(statusBar, 0, true);

    setLayout(layout);

    return true;
}

} // End of namespace LevelEditor
} // End of namespace Woden