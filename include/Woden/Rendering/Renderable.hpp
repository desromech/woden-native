#ifndef WODEN_RENDERING_RENDERABLE_HPP
#define WODEN_RENDERING_RENDERABLE_HPP


#include <memory>

namespace Woden
{
namespace SceneGraph
{
typedef std::shared_ptr<class SceneNode> SceneNodePtr;

} // End of namespace SceneGraph

namespace Rendering
{
typedef std::shared_ptr<class Renderable> RenderablePtr;
typedef std::shared_ptr<class RenderingScene> RenderingScenePtr;
class SceneRenderer;

class Renderable : public std::enable_shared_from_this<Renderable>
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene);
    virtual void renderShadowWith(SceneRenderer *renderer);
    virtual void renderDepthOnlyWith(SceneRenderer *renderer);
    virtual void renderOpaqueWith(SceneRenderer *renderer);
    virtual void renderTranslucentWith(SceneRenderer *renderer);
    
    SceneGraph::SceneNodePtr asSceneNode();
};

} // End of namespace Rendering
} // End of namespace Woden

#endif // WODEN_RENDERING_RENDERABLE_HPP