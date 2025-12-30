#ifndef WODEN_RENDERING_LIGHT_SOURCE
#define WODEN_RENDERING_LIGHT_SOURCE

#include <memory>

namespace Woden
{
namespace Rendering
{

typedef std::shared_ptr<class RenderingScene> RenderingScenePtr;

typedef std::shared_ptr<class LightSource> LightSourcePtr;
class LightSource
{
    void addIntoRenderingScene(const RenderingScenePtr &renderingScene);
};

} // End of namespace Rendering
} // End of namespace Woden

#endif // WODEN_RENDERING_LIGHT_SOURCE