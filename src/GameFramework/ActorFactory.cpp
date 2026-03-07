#include "Woden/GameFramework/ActorFactory.hpp"

namespace Woden
{
namespace GameFramework
{
 
ActorFactoryRegistryPtr ActorFactoryRegistryUniqueInstance;

ActorFactoryRegistryPtr ActorFactoryRegistry::Get()
{
    if(!ActorFactoryRegistryUniqueInstance)
        ActorFactoryRegistryUniqueInstance = std::make_shared<ActorFactoryRegistry> ();
    return ActorFactoryRegistryUniqueInstance;
}

void ActorFactoryRegistry::registerFactory(AbstractActorFactory *factory)
{
    classNameMap[factory->getClassName()] = factory;
}

AbstractActorFactory *ActorFactoryRegistry::findWithClassName(const std::string &className) const
{
    auto it = classNameMap.find(className);
    if(it != classNameMap.end())
        return it->second;
    return nullptr;
}

ActorPtr ActorFactoryRegistry::createWithClassNameOrFallbackToActor(const std::string &className) const
{
    auto factory = findWithClassName(className);
    ActorPtr actor;
    if(factory)
        actor = factory->makeActor();
    else
        actor = MakeActor<Actor> ();
    actor->className = className;
    return actor;
}

} // namespace GameFramework
} // namespace Woden