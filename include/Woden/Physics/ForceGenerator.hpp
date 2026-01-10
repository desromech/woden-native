#ifndef WODEN_PHYSICS_FORCE_GENERATOR_HPP
#define WODEN_PHYSICS_FORCE_GENERATOR_HPP

#include "Woden/Math/Scalar.hpp"
#include "Woden/Math/Vector3.hpp"
#include <memory>
#include <functional>

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

    virtual void worldAnchorPointsDo(const std::function<void (Math::Vector3)> &aBlock)
    {
        (void)aBlock;
    }

    PhysicsWorldWeakPtr owner;
};

} // End of namespace Physics
} // End of namespace Woden

#endif //WODEN_PHYSICS_FORCE_GENERATOR_HPP
