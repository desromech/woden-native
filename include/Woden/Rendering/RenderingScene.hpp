#ifndef WODEN_RENDERING_SCENE_HPP
#define WODEN_RENDERING_SCENE_HPP

#include "Renderable.hpp"
#include "Woden/Math/TRSTransform3D.hpp"
#include <vector>

namespace Woden
{
namespace Rendering
{

class RenderingSceneObject
{
public:
    uint32_t sceneObjectStateIndex;
    Math::Matrix4x4 modelMatrix;
    Math::Matrix4x4 inverseModelMatrix;
    RenderablePtr renderable;
};

class RenderingScene
{
public:
    template<typename FT>
    void withTRSTransformDuring(const Woden::Math::TRSTransform3D &transform, FT &&aBlock)
    {
        auto oldModelMatrix = currentModelMatrix;
        auto oldInverseModelMatrix = currentInverseModelMatrix;

        currentModelMatrix = currentModelMatrix * transform.asMatrix();
        currentInverseModelMatrix = transform.asInverseMatrix() * currentInverseModelMatrix;

        aBlock();
        
        currentModelMatrix = oldModelMatrix;
        currentInverseModelMatrix = oldInverseModelMatrix;
    }

    void addObjectWithRenderable(const RenderablePtr &renderable);

    Math::Matrix4x4 currentModelMatrix;
    Math::Matrix4x4 currentInverseModelMatrix;

    std::vector<RenderingSceneObject> backgroundObjects;
    std::vector<RenderingSceneObject> opaqueObjects;
    std::vector<RenderingSceneObject> translucentObject;
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_SCENE_RENDERING_ELEMENT_HPP