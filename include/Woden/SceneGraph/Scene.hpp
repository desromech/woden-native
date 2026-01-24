#ifndef WODEN_SCENE_GRAPH_SCENE_HPP
#define WODEN_SCENE_GRAPH_SCENE_HPP

#include "Woden/Math/TRSTransform3D.hpp"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Woden
{
namespace Morphic
{
typedef std::shared_ptr<class SceneMorph> SceneMorphPtr;
typedef std::shared_ptr<class SystemWindow> SystemWindowPtr;
}

namespace Rendering
{
typedef std::shared_ptr<class Camera> CameraPtr;
typedef std::shared_ptr<class Renderable> RenderablePtr;
typedef std::shared_ptr<class LightSource> LightSourcePtr;
typedef std::shared_ptr<class RenderingScene> RenderingScenePtr;
}

namespace SceneGraph
{

typedef std::shared_ptr<class SceneElement> SceneElementPtr;
typedef std::weak_ptr<class SceneElement> SceneElementWeakPtr;
typedef std::shared_ptr<class Scene> ScenePtr;
typedef std::shared_ptr<class SceneTreeElement> SceneTreeElementPtr;
typedef std::shared_ptr<class SceneTreeElementWithChildren> SceneTreeElementWithChildrenPtr;
typedef std::shared_ptr<class SceneLayer> SceneLayerPtr;
typedef std::shared_ptr<class SceneNode> SceneNodePtr;

/**
 * I am a scene element.
 */
class SceneElement : public std::enable_shared_from_this<SceneElement>
{
public:
    virtual void removeChild(SceneElementPtr child);
    virtual void addedToSceneElement(const SceneElementPtr &newParent);
    virtual void removedFromSceneElement(const SceneElementPtr &oldParent);
    virtual void addIntoRenderingScene(const Rendering::RenderingScenePtr &renderingScene);
    virtual ScenePtr getScene();

    virtual Math::Matrix4x4 computeGlobalTransformMatrix();

    std::string name;
};

/**
 * I am a scene.
 */
class Scene : public SceneElement
{
public:
    Scene();
    ~Scene();

    void initialize();
    virtual ScenePtr getScene() override;
    virtual void addIntoRenderingScene(const Rendering::RenderingScenePtr &renderingScene) override;

    Morphic::SceneMorphPtr openInMorphic();
    Morphic::SystemWindowPtr openInSystemWindow();

    SceneLayerPtr backgroundLayer;
    SceneLayerPtr normalLayer;
    SceneLayerPtr foregroundLayer;

    std::function<void (Math::Scalar delta)> updateFunction;
};

inline ScenePtr MakeScene()
{
    auto scene = std::make_shared<Scene> ();
    scene->initialize();
    return scene;
}
/**
 * I am a scene tree element.
 */
class SceneTreeElement : public SceneElement
{
public:
    virtual void removeFromParent();

    virtual void addedToSceneElement(const SceneElementPtr &newParent) override;
    virtual void removedFromSceneElement(const SceneElementPtr &oldParent) override;
    virtual ScenePtr getScene() override;
    virtual Math::Matrix4x4 computeGlobalTransformMatrix() override;

    SceneElementWeakPtr parent;
};

/**
 * I am a scene tree element with children.
 */
class SceneTreeElementWithChildren : public SceneTreeElement
{
public:
    void addChild(SceneElementPtr child);
    virtual void removeChild(SceneElementPtr child) override;
    void removeAllChildren();

    virtual void addIntoRenderingScene(const Rendering::RenderingScenePtr &renderingScene) override;

    std::vector<SceneElementPtr> children;
};

/**
 * I am a generic scene node
 */
class SceneNode : public SceneTreeElementWithChildren
{
public:
    void addCamera(const Rendering::CameraPtr &camera);
    void addLightSource(const Rendering::LightSourcePtr &lightSource);
    void addRenderable(const Rendering::RenderablePtr &renderable);
    virtual void addIntoRenderingScene(const Rendering::RenderingScenePtr &renderingScene) override;
    virtual Math::Matrix4x4 computeGlobalTransformMatrix() override;

    Math::TRSTransform3D transform;
    std::vector<Rendering::CameraPtr> cameras;
    std::vector<Rendering::RenderablePtr> renderables;
    std::vector<Rendering::LightSourcePtr> lightSources;
};

/**
 * I am a scene layer
 */
class SceneLayer : public SceneTreeElementWithChildren
{
public:
};

/**
 * I am a background scene layer
 */
class BackgroundSceneLayer : public SceneLayer
{
public:
};

/**
 * I am a normal scene layer
 */
class NormalSceneLayer : public SceneLayer
{
public:
};

/**
 * I am a foreground scene layer
 */
class ForegroundSceneLayer : public SceneLayer
{
public:
};

} // End of namespace SceneGraph
} // End of namespace Woden
#endif //WODEN_SCENE_SCENE_GRAPH_HPP