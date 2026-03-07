#ifndef WODEN_GAME_FRAMEWORK_ACTOR_FACTORY_HPP
#define WODEN_GAME_FRAMEWORK_ACTOR_FACTORY_HPP

#include <memory>
#include <unordered_map>
#include <string>

#include "Actor.hpp"

namespace Woden
{
namespace GameFramework
{

typedef std::shared_ptr<class Actor> ActorPtr;
typedef std::shared_ptr<class ActorFactoryRegistry> ActorFactoryRegistryPtr;

class AbstractActorFactory
{
public:
    virtual ActorPtr makeActor() const = 0;
    virtual const std::string &getClassName() const = 0;
};

/**
 * I am an actor factory registry.
 */
class ActorFactoryRegistry
{
public:
    static ActorFactoryRegistryPtr Get();

    void registerFactory(AbstractActorFactory *factory);
    AbstractActorFactory *findWithClassName(const std::string &className) const;
    ActorPtr createWithClassNameOrFallbackToActor(const std::string &className) const;

private:
    std::unordered_map<std::string, AbstractActorFactory*>  classNameMap;
};

template<typename AT>
class ActorFactory : public AbstractActorFactory
{
public:
    ActorFactory(const char *initClassName)
        : className(initClassName)
    {
        ActorFactoryRegistry::Get()->registerFactory(this);
    }

    virtual ActorPtr makeActor() const
    {
        return MakeActor<AT> ();
    }

    virtual const std::string &getClassName() const
    {
        return className;
    }

private:
    std::string className;
};


} // namespace GameFramework
} // namespace Woden

#endif // WODEN_GAME_FRAMEWORK_ACTOR_FACTORY_HPP