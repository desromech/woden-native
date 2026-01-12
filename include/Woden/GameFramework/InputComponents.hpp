#ifndef WODEN_GAME_FRAMEWORK_INPUT_COMPONENTS_HPP
#define WODEN_GAME_FRAMEWORK_INPUT_COMPONENTS_HPP

#include "ActorComponent.hpp"
#include "Woden/Events/Events.hpp"

namespace Woden
{
namespace GameFramework
{

using namespace Woden::Events;

typedef std::shared_ptr<class InputListenerComponent> InputListenerComponentPtr;

class InputListenerComponent : public ActorComponent
{
public:

    void dispatchEvent(const EventPtr &event);

    virtual void registerInWorld(const WorldPtr &world) override;

};
}
}
#endif // WODEN_GAME_FRAMEWORK_INPUT_COMPONENTS_HPP 