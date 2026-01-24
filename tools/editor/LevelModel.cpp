#include "LevelModel.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"

namespace Woden
{
namespace LevelEditor
{

// Level element
std::string LevelElement::asString() const
{
    return "[LE]" + name;
}

void LevelElement::addedToModel(const LevelModelPtr &newModel)
{
    model = newModel;
}

void LevelElement::removedFromModel(const LevelModelPtr &oldModel)
{
    (void)oldModel;
    model.reset();
}

// CSGBrush
void CSGBrush::addedToModel(const LevelModelPtr &model)
{
    LevelElement::addedToModel(model);

    assert(!sceneNode);
    
    auto mesh = makeMesh();
    sceneNode = mesh->asSceneNode();
    model->scene->normalLayer->addChild(sceneNode);
}

void CSGBrush::removedFromModel(const LevelModelPtr &model)
{
    LevelElement::removedFromModel(model);
    if(sceneNode)
    {
        sceneNode->removeFromParent();
        sceneNode.reset();
    }

}

// CSGBoxBrush
std::string CSGBoxBrush::asString() const
{
    return "[CSGBox]" + name;
}

Rendering::RenderablePtr CSGBoxBrush::makeMesh()
{
    return Woden::Rendering::MeshBuilder()
            .addCubeWithBox(boundingBox)
            .generateTexcoordsWithFacePlanarTransformWithScale(Math::Vector2(1, 1))
            .finishMesh();
}

// CSGBoxBrushPalette
std::string CSGBoxBrushPalette::asString() const
{
    return "CSG Box";
}

void CSGBoxBrushPalette::activate(const LevelModelPtr &model)
{
    auto box = std::make_shared<CSGBoxBrush> ();
    model->addElement(box);
}

// Level model.
void LevelModel::addElement(const LevelElementPtr &element)
{
    elements.push_back(element);
    element->addedToModel(shared_from_this());
}

Morphic::TableDataSourceElementPtr LevelModel::getElementAtIndex(size_t index)
{
    if(index < elements.size())
        return elements[index];
    return nullptr;
}

bool LevelModel::isValidIndex(size_t index)
{
    return index < elements.size();
}

size_t LevelModel::getNumberOfElements()
{
    return elements.size();
}

} // End of namespace LevelEditor
} // End of namespace Woden
