#ifndef WODEN_MORPHIC_SCENE_MORPH_HPP
#define WODEN_MORPHIC_SCENE_MORPH_HPP

#include "Morph.hpp"

namespace Woden
{
namespace GameFramework
{
typedef std::shared_ptr<class World> WorldPtr;
}

namespace Rendering
{
typedef std::shared_ptr<class SceneRenderer> SceneRendererPtr;
}

namespace SceneGraph
{
typedef std::shared_ptr<class Scene> ScenePtr;
typedef std::shared_ptr<class SceneNode> SceneNodePtr;
}

namespace Morphic
{
typedef std::shared_ptr<class WorldMorph> WorldMorphPtr;

/**
 * I am a world morph
 */
class WorldMorph : public Morph
{
public:
    WorldMorph();

    virtual void update(Math::Scalar deltaTime) override;
    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;

    SceneGraph::ScenePtr scene;
    SceneGraph::SceneNodePtr cameraNode;
    SceneGraph::SceneNodePtr sceneCameraNode;

    GameFramework::WorldPtr world;
    Rendering::SceneRendererPtr sceneRenderer;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_SCENE_MORPH