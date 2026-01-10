#ifndef WODEN_PHYSICS_FORCE_GENERATOR_HPP
#define WODEN_PHYSICS_FORCE_GENERATOR_HPP

#include "Woden/Math/Scalar.hpp"
#include <memory>

namespace Woden
{
namespace Physics
{

typedef std::shared_ptr<class ForceGenerator> ForceGeneratorPtr;
typedef std::weak_ptr<class PhysicsWorld> PhysicsWorldWeakPtr;

/**
 * I am a force generator. I am typically used for modeling constraints.
 */
class ForceGenerator
{
public:

    virtual void evaluateWithDeltaTime(Math::Scalar deltaTime)
    {
        (void)deltaTime;
    }

    PhysicsWorldWeakPtr owner;
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_FORCE_GENERATOR_HPP
