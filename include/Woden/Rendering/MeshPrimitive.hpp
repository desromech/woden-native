#ifndef WODEN_MESH_PRIMITIVE_HPP
#define WODEN_MESH_PRIMITIVE_HPP

#include "Woden/Assets/BinaryBuffer.hpp"
#include "Renderable.hpp"
#include "AGPU/agpu.hpp"

namespace Woden
{
namespace Rendering
{
typedef std::shared_ptr<class Material> MaterialPtr;
typedef std::shared_ptr<class MeshPrimitive> MeshPrimitivePtr;
typedef std::shared_ptr<class VertexBinding> VertexBindingPtr;

class MeshPrimitive : public Renderable
{
public:
    virtual void addIntoRenderingScene(const RenderingScenePtr &renderingScene) override;

    virtual void renderShadowWith(SceneRenderer *renderer) override;
    virtual void renderDepthOnlyWith(SceneRenderer *renderer) override;
    virtual void renderOpaqueWith(SceneRenderer *renderer) override;

    MaterialPtr getValidMaterial() const;

    agpu_primitive_topology topology = AGPU_TRIANGLES;
    MaterialPtr material;
    VertexBindingPtr vertexBinding;
    Assets::BinaryBufferAccessorPtr indices;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_MESH_PRIMITIVE_HPP