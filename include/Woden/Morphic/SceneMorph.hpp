#ifndef WODEN_MORPHIC_SCENE_MORPH_HPP
#define WODEN_MORPHIC_SCENE_MORPH_HPP

#include "Morph.hpp"

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

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;

    SceneGraph::ScenePtr scene;
    SceneGraph::SceneNodePtr cameraNode;
    Rendering::SceneRendererPtr sceneRenderer;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_SCENE_MORPH