#include "Woden/GameFramework/InputSubsystem.hpp"
#include "Woden/GameFramework/InputComponents.hpp"

namespace Woden
{
namespace GameFramework
{

void InputSubsystem::dispatchEvent(const EventPtr &event)
{
    for(auto &listener : inputListeners)
        listener->dispatchEvent(event);
}

void InputSubsystem::registerInputListener(const InputListenerComponentPtr &newListener)
{
    inputListeners.push_back(newListener);
}

void InputSubsystem::unregisterInputListener(const InputListenerComponentPtr &oldListener)
{
    for(size_t i = 0; i < inputListeners.size(); ++i)
    {
        if(inputListeners[i] == oldListener)
        {
            inputListeners.erase(inputListeners.begin() + i);
            return;
        }
    }
}

} // End of namespace GameFramework
} // End of namespace Woden 