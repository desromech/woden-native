#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Assets/QMapFile.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Morphic/Morph.hpp"
#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::Morphic;
using namespace Woden::SceneGraph;

void printHelp()
{
    printf("MapViewer <mapfile.map>\n");
}

void printVersion()
{
    printf("MapViewer Version 0.1\n");
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

    auto scene = MakeScene();
    if(mapFile)
    {
        mapFile->computeGeometry();
        mapFile->addToSceneWithInverseScale(scene, 32);
    }
    
    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8f, 0.8f, 0.2f);
        pointLightSource->intensity = 10;
        pointLightSource->influenceRadius = 10;

        scene->normalLayer->addChild(pointLightSource->asSceneNodeWithPosition(Vector3(-1.5f, 3.0, 1.6f)));

    }

    scene->openInSystemWindow();

    int exitCode = Morph::runMainLoop();
    Woden::Assets::ResourceCache::releaseSingleton();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}
