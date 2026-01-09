#include "Woden/Physics/ContactPoint.hpp"
#include "Woden/Physics/CollisionObject.hpp"

namespace Woden
{
namespace Physics
{

bool ContactPoint::firstHasCollisionResponse() const
{
    return int(firstObject->collisionResponseGroupMask) & int(secondObject->collisionGroup);
}

bool ContactPoint::secondHasCollisionResponse() const
{
    return int(secondObject->collisionResponseGroupMask) & int(firstObject->collisionGroup);
}

Math::Scalar ContactPoint::inverseInertia() const
{
    return inverseLinearInertia() + inverseAngularInertia();
}

Math::Scalar ContactPoint::inverseLinearInertia() const
{
    return firstObject->getInverseMass() + secondObject->getInverseMass();
}

Math::Scalar ContactPoint::inverseAngularInertia() const
{
    return firstObject->computeAngularInertiaForRelativeContactPoint(getRelativeFirstPoint(), normal)
        + secondObject->computeAngularInertiaForRelativeContactPoint(getRelativeSecondPoint(), -normal);
}

Math::Vector3 ContactPoint::getRelativeFirstPoint() const
{
    return firstPoint - firstObject->getPosition();
}

Math::Vector3 ContactPoint::getRelativeSecondPoint() const
{
    return secondPoint - secondObject->getPosition();
}

Math::Matrix3x3 ContactPoint::computeContactSpaceMatrix() const
{
    auto x = normal;
    auto y = Math::Vector3(1, 0, 0);
    if(abs(normal.x) > abs(normal.y))
        y = Math::Vector3(0, 1, 0);

    auto z = x.cross(y).normalized();
    y = z.cross(x).normalized();
    
    return Math::Matrix3x3::WithColumns(x, y, z);
}

void ContactPoint::computeLocalVersionWithTransforms(const Math::RigidTransform &firstTransform, const Math::RigidTransform &secondTransform)
{
    localFirstPoint = firstTransform.inverseTransformPosition(firstPoint);
	localSecondPoint = secondTransform.inverseTransformPosition(secondPoint);
    localFirstNormal = firstTransform.inverseTransformNormalVector(secondTransform.inverseTransformNormalVector(normal));
    localSecondNormal = secondTransform.inverseTransformNormalVector(firstTransform.inverseTransformNormalVector(-normal));
}

void ContactPoint::computeWorldVersionsWithTransforms(const Math::RigidTransform &firstTransform, const Math::RigidTransform &secondTransform)
{
    firstPoint = firstTransform.transformPosition(localFirstPoint);
    secondPoint = secondTransform.transformPosition(localSecondPoint);
    normal = secondTransform.transformNormalVector(firstTransform.transformNormalVector(localFirstNormal));
    computeWorldContactPointAndDistances();
}

void ContactPoint::update()
{
    computeWorldVersionsWithTransforms(firstObject->getTransform(), secondObject->getTransform());
}



} // End of namespace Physics
} // End of namespace Woden