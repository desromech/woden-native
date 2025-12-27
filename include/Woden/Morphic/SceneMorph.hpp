#ifndef WODEN_MORPHIC_SCENE_MORPH_HPP
#define WODEN_MORPHIC_SCENE_MORPH_HPP

#include "Morph.hpp"

namespace Woden
{
namespace SceneGraph
{

typedef std::shared_ptr<class Scene> ScenePtr;
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
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_SCENE_MORPH