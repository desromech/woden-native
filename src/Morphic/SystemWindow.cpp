#include "Woden/Morphic/SystemWindow.hpp"
#include <assert.h>
#include <map>

namespace Woden
{
namespace Morphic
{

static bool hasInitializedSDL2 = false;
static bool isQuitting = false;
static std::map<uint32_t, SystemWindowPtr> sdlWindowMap;

void ensureSDLInitialization()
{
    if(hasInitializedSDL2)
        return;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
    hasInitializedSDL2 = true;
}

void processEvent(const SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_QUIT:
        isQuitting = true;
        break;
    default:
        // Ignored
        break;
    }
}

void processEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
        processEvent(&event);
}

int Morph::runMainLoop()
{
    ensureSDLInitialization();
    while(sdlWindowMap.size() > 0 && !isQuitting)
    {
        processEvents();

        for(auto &handleAndWindow : sdlWindowMap)
            handleAndWindow.second->updateAndRender();
    }
    return 0;
}

SystemWindowPtr Morph::openInSystemWindow()
{
    auto systemWindow = std::make_shared<SystemWindow> ();
    systemWindow->bounds = Rectangle(Vector2(0, 0), bounds.extent());
    systemWindow->open();

    return systemWindow;
}

void SystemWindow::open()
{
    if(handle)
        return;

    ensureSDLInitialization();

    auto extent = bounds.extent();
    handle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, int(extent.x + 0.5), int(extent.y + 0.5), SDL_WINDOW_SHOWN);

    auto windowId = SDL_GetWindowID(handle);
    sdlWindowMap.insert(std::make_pair(windowId, std::static_pointer_cast<SystemWindow> (shared_from_this())));
}

void SystemWindow::close()
{
    if(!handle)
        return;
}

void SystemWindow::updateAndRender()
{
    //printf("TODO: update and render\n");
}

} // End of namespace Morphic
} // End of namespace Woden