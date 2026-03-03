#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Assets/QMapFile.hpp"
#include "Woden/GameFramework/Actor.hpp"
#include "Woden/GameFramework/ActorSceneComponents.hpp"
#include "Woden/GameFramework/PlayerActor.hpp"
#include "Woden/GameFramework/World.hpp"

#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/Morph.hpp"
#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::GameFramework;
using namespace Woden::Morphic;
using namespace Woden::SceneGraph;

void printHelp()
{
    printf("MapPlayer <mapfile.map>\n");
}

void printVersion()
{
    printf("MapPlayer Version 0.1\n");
}

int main(int argc, const char *argv[])
{
    if(!Woden::Rendering::RenderingContext::getOrCreateMainContext(argc, argv))
        return 1;

    if(!Woden::Assets::ResourceCache::Get()->initialize())
        return 1;

    Woden::Assets::QMapFilePtr mapFile;

    for(int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if(arg[0] == '-') 
        {
            if(arg == "-help")
            {
                printHelp();
                return 0;
            }
            else if(arg == "version")
            {
                printVersion();
                return 0;
            }
        }
        else
        {
            mapFile = Woden::Assets::QMapFile::parseFromFileNamed(arg);
        }
    }

    auto world = std::make_shared<World> ();

    if(mapFile)
    {
        mapFile->computeGeometry();
        mapFile->addToWorldWithInverseScale(world, 32);
    }

    auto infoPlayerStart = world->findActorWithClassName("info_player_start");
    if(infoPlayerStart)
    {
        auto playerActor = MakeActor<PlayerActor> ();
        playerActor->setPosition(infoPlayerStart->getPosition());
        world->spawnActor(playerActor);
    }

    world->playInSystemWindow();
    
    int exitCode = Morph::runMainLoop();
    Woden::Assets::ResourceCache::releaseSingleton();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}
