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
    createMenuBar();
    toolBar = std::make_shared<ToolBarMorph> ();
    sceneView = std::make_shared<Morph> ();
    statusBar = std::make_shared<StatusBarMorph> ();

    sceneView->color = Vector4(0.1, 1, 0.1, 1);

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

void LevelEditorMorph::createMenuBar()
{
    menuBar = std::make_shared<MenuBarMorph> ();
    menuBar->addItem("File", nullptr);
    menuBar->addItem("Edit", nullptr);
    menuBar->addItem("Game", nullptr);
    menuBar->addItem("View", nullptr);
    menuBar->addItem("Help", nullptr);
}

} // End of namespace LevelEditor
} // End of namespace Woden