#ifndef WODEN_RENDERING_MATERIAL_HPP
#define WODEN_RENDERING_MATERIAL_HPP

#include "AGPU/agpu.hpp"
#include <memory>

namespace Woden
{
namespace Rendering
{
typedef std::shared_ptr<class Material> MaterialPtr;
class SceneRenderer;

/**
 * I am a material.
 */
class Material
{
public:
    static MaterialPtr getDefaultMaterial();

    virtual bool activateDepthOnlyWithRenderer(SceneRenderer *sceneRenderer) = 0;
    virtual bool activateShadowMapWithRenderer(SceneRenderer *sceneRenderer) = 0;
    virtual bool activateOpaqueWithRenderer(SceneRenderer *sceneRenderer) = 0;
    virtual bool activateTranslucentWithRenderer(SceneRenderer *sceneRenderer) = 0;

    agpu_shader_resource_binding_ref materialResourceBinding;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_MATERIAL_HPP
