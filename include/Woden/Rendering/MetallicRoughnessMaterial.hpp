#ifndef WODEN_RENDERING_METALLIC_ROUGHNESS_MATERIAL_HPP
#define WODEN_RENDERING_METALLIC_ROUGHNESS_MATERIAL_HPP

#include "SurfaceMaterial.hpp"
#include "Woden/Assets/Texture.hpp"
#include "Woden/Math/Vector2.hpp"
#include "Woden/Math/Vector3.hpp"
#include "Woden/Math/Vector4.hpp"

namespace Woden
{
namespace Rendering
{

typedef std::shared_ptr<class MetallicRoughnessMaterial> MetallicRoughnessMaterialPtr;

/**
 * I am a metallic-roughness material
 */
class MetallicRoughnessMaterial : public SurfaceMaterial
{
public:
    virtual bool activateDepthOnlyWithRenderer(SceneRenderer *sceneRenderer) override;
    virtual bool activateShadowMapWithRenderer(SceneRenderer *sceneRenderer) override;
    virtual bool activateOpaqueWithRenderer(SceneRenderer *sceneRenderer) override;
    virtual bool activateTranslucentWithRenderer(SceneRenderer *sceneRenderer) override;

    Math::Vector4 baseColorFactor = Math::Vector4(1, 1, 1, 1);
    Math::Vector3 emissiveFactor = Math::Vector3(0, 0, 0);

    Math::Scalar occlusionFactor = 1;
    Math::Scalar roughnessFactor = 1;
    Math::Scalar metallicFactor = 1;
    
    Math::Vector2 texcoordOffset = Math::Vector2(0, 0);
    Math::Vector2 texcoordScale = Math::Vector2(1, 1);
    Math::Vector2 texcoordOffsetVelocity = Math::Vector2(0, 0);

    Assets::TexturePtr baseColorTexture;
    Assets::TexturePtr emissiveTexture;
    Assets::TexturePtr normalTexture;
    Assets::TexturePtr occlusionTexture;
    Assets::TexturePtr metallicRoughnessTexture;

    bool hasChanged = false;

    void changed()
    {
        hasChanged = true;
    }

private:
    agpu_shader_resource_binding_ref getValidResourceBinding(SceneRenderer *sceneRenderer);
    struct SceneMetallicRoughnessMaterial *materialState = nullptr;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_RENDERING_METALLIC_ROUGHNESS_MATERIAL_HPP
