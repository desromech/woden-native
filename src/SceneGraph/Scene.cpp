#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Morphic/SceneMorph.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/LightSource.hpp"

namespace Woden
{
namespace SceneGraph
{

// Scene element
void SceneElement::addedToSceneElement(const SceneElementPtr &newParent)
{
    assert(0 && "Element does not support having a parent.");
}

void SceneElement::addIntoRenderingScene(const Rendering::RenderingScenePtr &renderingScene)
{
    (void)renderingScene;
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

void Scene::addIntoRenderingScene(const Rendering::RenderingScenePtr &renderingScene)
{
    backgroundLayer->addIntoRenderingScene(renderingScene);
    normalLayer->addIntoRenderingScene(renderingScene);
    foregroundLayer->addIntoRenderingScene(renderingScene);
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

void SceneTreeElementWithChildren::addIntoRenderingScene(const Rendering::RenderingScenePtr &renderingScene)
{
    for(auto &child : children)
        child->addIntoRenderingScene(renderingScene);
}

// Scene node
void SceneNode::addCamera(const Rendering::CameraPtr &camera)
{
    cameras.push_back(camera);
}

void SceneNode::addLightSource(const Rendering::LightSourcePtr &lightSource)
{
    lightSources.push_back(lightSource);
}

void SceneNode::addRenderable(const Rendering::RenderablePtr &renderable)
{
    renderables.push_back(renderable);
}

void SceneNode::addIntoRenderingScene(const Rendering::RenderingScenePtr &renderingScene)
{
    renderingScene->withTRSTransformDuring(transform, [&](){
        for(auto &renderable : renderables)
            renderable->addIntoRenderingScene(renderingScene);
        for(auto &lightSource : lightSources)
            lightSource->addIntoRenderingScene(renderingScene);
        SceneTreeElementWithChildren::addIntoRenderingScene(renderingScene);
    });
}

} // End of namespace SceneGraph
} // End of namespace Woden