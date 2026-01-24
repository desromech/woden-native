#include "LevelEditorMorph.hpp"
#include "Woden/Morphic/Layout.hpp"
#include "Woden/Morphic/RootMorph.hpp"

#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/Rendering/InfiniteGrid.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace LevelEditor
{

LevelEditorMorph::LevelEditorMorph()
{
    setExtent(Vector2(1024, 576));
}

void LevelEditorMorph::initialize()
{
    createMenuBar();
    model = std::make_shared<LevelModel> ();

    toolBar = MakeMorph<ToolBarMorph> ();
    sceneView = MakeMorph<LevelEditorSceneViewMorph> ();
    statusBar = MakeMorph<StatusBarMorph> ();
    levelElementsTable = MakeMorph<TableMorph> ();
    objectPaletteTable = MakeMorph<TableMorph> ();

    levelElementsTable->setDataSource(model);
    buildObjectPalette();

    addMorph(menuBar);
    addMorph(toolBar);
    addMorph(levelElementsTable);
    addMorph(objectPaletteTable);
    addMorph(sceneView);
    addMorph(statusBar);

    auto leftLayout = std::make_shared<VerticalPackingLayout> ();
    leftLayout->addMorph(levelElementsTable, 1, true);
    leftLayout->addMorph(objectPaletteTable, 1, true);

    auto middleLayout = std::make_shared<HorizontalPackingLayout> ();
    middleLayout->addLayout(leftLayout, 1, true);
    middleLayout->addMorph(sceneView, 4, true);

    auto layout = std::make_shared<VerticalPackingLayout> ();
    layout->addMorph(menuBar, 0, true);
    layout->addMorph(toolBar, 0, true);
    layout->addLayout(middleLayout, 1, true);
    layout->addMorph(statusBar, 0, true);

    setLayout(layout);

    auto scene = SceneGraph::MakeScene();
    model->scene = scene;

    // Infinite grid
    {
        auto grid = std::make_shared<Rendering::InfiniteGridRenderable> ();
        scene->normalLayer->addChild(grid->asSceneNode());
    }
    
    /*// Cube
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
    }*/
    
    sceneView->scene = scene;
    sceneView->cameraNode->transform.translation = Vector3(0, 1, 3);
}

void LevelEditorMorph::buildObjectPalette()
{
    auto dataSource = std::make_shared<SimpleTableDataSource> ();
    dataSource->addElement(std::make_shared<CSGBoxBrushPalette> ());
    objectPaletteTable->setDataSource(dataSource);
    objectPaletteTable->onElementActivate = [=](TableDataSourceElementPtr sourceElement) {
        auto objectPalette = std::static_pointer_cast<ObjectPaletteElement> (sourceElement);
        objectPalette->activate(model);
    };
}

void LevelEditorMorph::createMenuBar()
{
    menuBar = MakeMorph<MenuBarMorph> ();
    {
        auto menu = MakeMorph<MenuMorph> ();
        menu->addItem("Open File...", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: Open file\n");
        });
        menu->addItem("Save File...", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: Save file\n");
        });
        menu->addItem("Exit", [=](const MorphPtr &eventSource){
            (void)eventSource;
            getRootMorph()->close();
        });

        menuBar->addItem("File", menu);
    }
    {
        auto menu = MakeMorph<MenuMorph> ();
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
        auto menu = MakeMorph<MenuMorph> ();
        menu->addItem("Play Level", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: Play Level\n");
        });

        menuBar->addItem("Game", menu);
    }

    {
        auto menu = MakeMorph<MenuMorph> ();
        menuBar->addItem("View", menu);
    }
    
    {
        auto menu = MakeMorph<MenuMorph> ();
        menu->addItem("About", [=](const MorphPtr &eventSource){
            (void)eventSource;
            printf("TODO: About window\n");
        });

        menuBar->addItem("Help", menu);
    }
}

} // End of namespace LevelEditor
} // End of namespace Woden