#ifndef WODEN_STATIC_MESH_PRIMITIVE_HPP
#define WODEN_STATIC_MESH_PRIMITIVE_HPP

#include "Woden/Rendering/Renderable.hpp"

namespace Woden
{
namespace Rendering
{
typedef std::shared_ptr<class Renderable> RenderablePtr;

class StaticMeshRenderable : public Renderable
{
public:
};

} // End of namespace Rendering
} // End of namespace Woden

#endif //WODEN_STATIC_MESH_PRIMITIVE_HPP