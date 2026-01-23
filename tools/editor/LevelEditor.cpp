#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Morphic/Morph.hpp"
#include "LevelEditorMorph.hpp"

using namespace Woden::Morphic;

int main(int argc, const char *argv[])
{
    if(!Woden::Rendering::RenderingContext::getOrCreateMainContext(argc, argv))
        return 1;

    if(!Woden::Assets::ResourceCache::Get()->initialize())
        return 1;
    
    auto editor = MakeMorph<Woden::LevelEditor::LevelEditorMorph> ();
    editor->openInSystemWindow();

    int exitCode = Morph::runMainLoop();
    Woden::Assets::ResourceCache::releaseSingleton();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}
