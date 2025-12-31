#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/Morph.hpp"
#include "SampleCommon.hpp"

using namespace Woden::Morphic;

int main(int argc, const char *argv[])
{
    if(!Woden::Rendering::RenderingContext::getOrCreateMainContext(argc, argv))
        return 1;

    if(!Woden::Assets::ResourceCache::Get()->initialize())
        return 1;

    int exitCode = woden_main(argc, argv);
    if(exitCode != 0)
        return exitCode;

    exitCode = Morph::runMainLoop();
    Woden::Assets::ResourceCache::releaseSingleton();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}
