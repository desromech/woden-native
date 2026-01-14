#include "LevelEditorMorph.hpp"
#include "Woden/Morphic/Layout.hpp"

#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/SceneGraph/Scene.hpp"

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
    sceneView = std::make_shared<SceneMorph> ();
    statusBar = std::make_shared<StatusBarMorph> ();

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

    auto scene = SceneGraph::MakeScene();
    {
        scene->normalLayer->addChild(Woden::Rendering::MeshBuilder()
            .addCubeWithExtent(Vector3(1, 1, 1))
            .generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1))
            .finishMesh()->asSceneNode()
        );
    }

    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8f, 0.8f, 0.2f);
        pointLightSource->intensity = 5;
        pointLightSource->influenceRadius = 4;

        scene->normalLayer->addChild(pointLightSource->asSceneNodeWithPosition(Vector3(-1.5f, 1.5f, 1.6f)));
    }
    sceneView->scene = scene;

    return true;
}

void LevelEditorMorph::createMenuBar()
{
    menuBar = std::make_shared<MenuBarMorph> ();
    {
        auto menu = std::make_shared<MenuMorph> ();
        menu->addItem("Open File...", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: Open file\n");
        });
        menu->addItem("Exit", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: Exit\n");
        });

        menuBar->addItem("File", menu);
    }
    {
        auto menu = std::make_shared<MenuMorph> ();
        menu->addItem("Undo", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: Undo\n");
        });
        menu->addItem("Redo", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: Redo\n");
        });

        menuBar->addItem("Edit", menu);
    }
    {
        auto menu = std::make_shared<MenuMorph> ();
        menu->addItem("Play Level", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: Play Level\n");
        });

        menuBar->addItem("Game", menu);
    }

    {
        auto menu = std::make_shared<MenuMorph> ();
        menuBar->addItem("View", menu);
    }
    
    {
        auto menu = std::make_shared<MenuMorph> ();
        menu->addItem("About", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: About window\n");
        });

        menuBar->addItem("Help", menu);
    }
}

} // End of namespace LevelEditor
} // End of namespace Woden