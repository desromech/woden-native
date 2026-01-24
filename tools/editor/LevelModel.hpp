#ifndef WODEN_EDITOR_LEVEl_MODEL_HPP
#define WODEN_EDITOR_LEVEl_MODEL_HPP

#include "Woden/Math/AABox.hpp"
#include "Woden/Math/Ray3D.hpp"
#include "Woden/Morphic/TableMorph.hpp"
#include "Woden/SceneGraph/Scene.hpp"
#include <memory>
#include <vector>

namespace Woden
{
namespace LevelEditor
{

typedef std::shared_ptr<class LevelModel> LevelModelPtr;
typedef std::weak_ptr<class LevelModel> LevelModelWeakPtr;
typedef std::shared_ptr<class LevelElement> LevelElementPtr;

struct ElementAlongRay
{
    LevelElementPtr element;
    Math::Scalar rayDistance;
};

class ObjectPaletteElement : public Morphic::TableDataSourceElement
{
public:
    virtual std::string asString() const = 0;
    virtual void activate(const LevelModelPtr &model) = 0;
};

class LevelElement : public Morphic::TableDataSourceElement, public std::enable_shared_from_this<LevelElement>
{
public:
    virtual std::string asString() const override;

    virtual void addedToModel(const LevelModelPtr &model);
    virtual void removedFromModel(const LevelModelPtr &model);

    virtual void addElementsAlongRay(const Math::Ray3D &ray, std::vector<ElementAlongRay> &elementsAlongRay);

    LevelModelWeakPtr model;
    std::string name;
};

class CSGBrush : public LevelElement
{
public:
    virtual void addedToModel(const LevelModelPtr &model) override;
    virtual void removedFromModel(const LevelModelPtr &model) override;

    virtual Rendering::RenderablePtr makeMesh() = 0;

    SceneGraph::SceneNodePtr sceneNode;
};

class CSGBoxBrush : public CSGBrush
{
public:
    virtual std::string asString() const override;
    virtual Rendering::RenderablePtr makeMesh() override;

    virtual void addElementsAlongRay(const Math::Ray3D &ray, std::vector<ElementAlongRay> &elementsAlongRay) override;

    Math::AABox boundingBox = Math::AABox::WithHalfExtent(0.5f);
};

class CSGBoxBrushPalette : public ObjectPaletteElement
{
public:
    virtual std::string asString() const override;
    virtual void activate(const LevelModelPtr &model) override;
};



class LevelModel : public Morphic::TableDataSource, public std::enable_shared_from_this<LevelModel>
{
public:
    void addElement(const LevelElementPtr &element);

    virtual Morphic::TableDataSourceElementPtr getElementAtIndex(size_t index) override;
    virtual bool isValidIndex(size_t index) override;

    virtual size_t getNumberOfElements() override;

    std::vector<ElementAlongRay> findElementsAlongRay(const Math::Ray3D &ray);

    SceneGraph::ScenePtr scene;
    std::vector<LevelElementPtr> elements;
};

} // End of namespace LevelEditor
} // End of namespace Woden

#endif //WODEN_EDITOR_LEVEl_MODEL_HPP