#include "Woden/Physics/ContactManifold.hpp"

namespace Woden
{
namespace Physics
{

Math::Vector3 ContactManifoldCache::getLastSeparatingAxis(const CollisionObjectPtr &first, const CollisionObjectPtr &second)
{
    // TODO: implement this properly.
    return Math::Vector3(1, 0, 0);
}

} // End of namespace Physics
} // End of namespace Woden
