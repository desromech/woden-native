#ifndef WODEN_MORPHIC_SCENE_MORPH_HPP
#define WODEN_MORPHIC_SCENE_MORPH_HPP

#include "Morph.hpp"
#include "Woden/Math/Quaternion.hpp"

namespace Woden
{
namespace SceneGraph
{
typedef std::shared_ptr<class Scene> ScenePtr;
typedef std::shared_ptr<class SceneNode> SceneNodePtr;
}

namespace Rendering
{
typedef std::shared_ptr<class SceneRenderer> SceneRendererPtr;
}

namespace Morphic
{
typedef std::shared_ptr<class SceneMorph> SceneMorphPtr;

/**
 * I am morph
 */
class SceneMorph : public Morph
{
public:
    SceneMorph();

    virtual void update(Math::Scalar deltaTime) override;
    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;

    virtual void handleMouseMotionEvent(const MouseMotionEventPtr &event);
    virtual void handleMouseWheelEvent(const MouseWheelEventPtr &event);

    Math::Quaternion computeCameraOrientation() const;

    SceneGraph::ScenePtr scene;
    SceneGraph::SceneNodePtr cameraNode;
    Math::Vector3 cameraAngles = Vector3();
    Rendering::SceneRendererPtr sceneRenderer;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_SCENE_MORPH