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
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) = 0;
};

class DirectionalLightSource : public LightSource
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) override;
};

class PointLightSource : public LightSource
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) override;
};

class SpotLightSource : public PointLightSource
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) override;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif // WODEN_RENDERING_LIGHT_SOURCE