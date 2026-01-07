#include "Woden/Physics/ContactPoint.hpp"
#include "Woden/Physics/CollisionObject.hpp"

namespace Woden
{
namespace Physics
{

Math::Scalar ContactPoint::inverseInertia() const
{
    return inverseLinearInertia();
}

Math::Scalar ContactPoint::inverseLinearInertia() const
{
    return firstObject->getInverseMass() + secondObject->getInverseMass();
}

Math::Vector3 ContactPoint::getRelativeFirstPoint() const
{
    return firstPoint - firstObject->getPosition();
}

Math::Vector3 ContactPoint::getRelativeSecondPoint() const
{
    return secondPoint - secondObject->getPosition();
}

} // End of namespace Physics
} // End of namespace Woden