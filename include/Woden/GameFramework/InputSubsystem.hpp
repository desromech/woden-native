#ifndef WODEN_GAME_FRAMEWORK_INPUT_SUBSYSTEM_HPP
#define WODEN_GAME_FRAMEWORK_INPUT_SUBSYSTEM_HPP

#include "Subsystem.hpp"
#include "Woden/Events/Events.hpp"
#include <vector>

namespace Woden
{
namespace GameFramework
{

using namespace Woden::Events;
typedef std::shared_ptr<class InputListenerComponent> InputListenerComponentPtr;

/**
 * I am a subsystem used for listening to input events.
 */
class InputSubsystem : public Subsystem
{
public:
    void dispatchEvent(const EventPtr &event);

    void registerInputListener(const InputListenerComponentPtr &newListener);
    void unregisterInputListener(const InputListenerComponentPtr &oldListener);

    std::vector<InputListenerComponentPtr> inputListeners;
};

}
}
#endif //WODEN_GAME_FRAMEWORK_INPUT_SUBSYSTEM_HPP
