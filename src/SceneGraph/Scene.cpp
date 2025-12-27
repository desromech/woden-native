#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Morphic/SceneMorph.hpp"

namespace Woden
{
namespace SceneGraph
{

// Scene element
void SceneElement::addedToSceneElement(const SceneElementPtr &newParent)
{
    assert(0 && "Element does not support having a parent.");
}

ScenePtr SceneElement::getScene()
{
    return nullptr;
}

// Scene
Scene::Scene()
{
}

void Scene::initialize()
{
    {
        auto layer = std::make_shared<BackgroundSceneLayer> ();
        layer->addedToSceneElement(shared_from_this());
        backgroundLayer = layer;
    }

    {
        auto layer = std::make_shared<NormalSceneLayer> ();
        layer->addedToSceneElement(shared_from_this());
        normalLayer = layer;
    }

    {
        auto layer = std::make_shared<ForegroundSceneLayer> ();
        layer->addedToSceneElement(shared_from_this());
        foregroundLayer = layer;
    }
}

Scene::~Scene()
{
}

ScenePtr Scene::getScene()
{
    return std::static_pointer_cast<Scene> (shared_from_this());
}

Morphic::SceneMorphPtr Scene::openInMorphic()
{
    auto sceneMorph = std::make_shared<Morphic::SceneMorph> ();
    sceneMorph->scene = std::static_pointer_cast<Scene> (shared_from_this());
    return sceneMorph;
}

Morphic::SystemWindowPtr Scene::openInSystemWindow()
{
    auto sceneMorph = openInMorphic();
    return sceneMorph->openInSystemWindow();
}

// SceneTree Element
void SceneTreeElement::addedToSceneElement(const SceneElementPtr &newParent)
{
    assert(!parent.lock());
    parent = newParent;
}

ScenePtr SceneTreeElement::getScene()
{
    auto p = parent.lock();
    if(!p) return nullptr;

    return p->getScene();
}

// SceneTreeElementWithChildren
void SceneTreeElementWithChildren::addChild(SceneElementPtr child)
{
    child->addedToSceneElement(shared_from_this());
    children.push_back(child);
}

} // End of namespace SceneGraph
} // End of namespace Woden