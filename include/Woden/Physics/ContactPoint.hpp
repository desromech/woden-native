#ifndef WODEN_PHYSICS_CONTACT_POINT_HPP
#define WODEN_PHYSICS_CONTACT_POINT_HPP

#include "Woden/Math/Vector3.hpp"

namespace Woden
{
namespace Physics
{

/**
 * I am a single contact point.
 */
struct ContactPoint
{
    Math::Vector3 normal;
    Math::Scalar requiredSeparation = 0;
    Math::Vector3 firstPoint;
    Math::Vector3 secondPoint;

    void flip()
    {
        normal = -normal;
        std::swap(firstPoint, secondPoint);
    }
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_CONTACT_POINT_HPP