#include "Woden/Morphic/SystemWindow.hpp"
#include "Woden/Morphic/Layout.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"
#include "Woden/Utility/Time.hpp"
#include "SDL_syswm.h"
#include <assert.h>
#include <map>

namespace Woden
{
namespace Morphic
{

static bool hasInitializedSDL2 = false;
static bool isQuitting = false;
static std::map<uint32_t, SystemWindowPtr> sdlWindowMap;

static void ensureSDLInitialization()
{
    if(hasInitializedSDL2)
        return;

    SDL_SetHint(SDL_HINT_NO_SIGNAL_HANDLERS, "1");
    //SDL_SetHint(SDL_HINT_QUIT_ON_LAST_WINDOW_CLOSE, "0");
        
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
    hasInitializedSDL2 = true;
}

static SystemWindowPtr getWindowWithID(uint32_t id)
{
    auto it = sdlWindowMap.find(id);
    if(it == sdlWindowMap.end())
        return nullptr;
    return it->second;
}

static void onKeyboardDown(const SDL_KeyboardEvent &event)
{
    auto window = getWindowWithID(event.windowID);
    if(!window)
        return;

    auto morphicEvent = std::make_shared<KeyboardDownEvent> ();
    morphicEvent->keySymbol = event.keysym.sym;
    morphicEvent->isKeyRepeat =  event.repeat ? true : false;
    window->processKeyboardEvent(morphicEvent);
}

static void onKeyboardUp(const SDL_KeyboardEvent &event)
{
    auto window = getWindowWithID(event.windowID);
    if(!window)
        return;

    auto morphicEvent = std::make_shared<KeyboardUpEvent> ();
    morphicEvent->keySymbol = event.keysym.sym;
    morphicEvent->isKeyRepeat =  event.repeat ? true : false;
    window->processKeyboardEvent(morphicEvent);
}

static void onTextInput(const SDL_TextInputEvent &event)
{
    auto window = getWindowWithID(event.windowID);
    if(!window)
        return;

    auto morphicEvent = std::make_shared<TextInputEvent> ();
    morphicEvent->text = event.text;
    window->processKeyboardEvent(morphicEvent);
}


static void onMouseButtonDown(const SDL_MouseButtonEvent &event)
{
    auto window = getWindowWithID(event.windowID);
    if(!window)
        return;

    auto morphicEvent = std::make_shared<MouseButtonDownEvent> ();
    morphicEvent->position = Vector2(event.x, event.y);
    morphicEvent->buttonIndex = event.button;

    window->processEvent(morphicEvent);
    if(event.clicks == 1)
    {
        auto clickEvent = std::make_shared<MouseClickEvent> ();
        clickEvent->position = Vector2(event.x, event.y);
        clickEvent->buttonIndex = event.button;
        window->processEvent(clickEvent);
    }
    else if(event.clicks == 2)
    {
        auto clickEvent = std::make_shared<MouseDoubleClickEvent> ();
        clickEvent->position = Vector2(event.x, event.y);
        clickEvent->buttonIndex = event.button;
        window->processEvent(clickEvent);
    }
}

static void onMouseButtonUp(const SDL_MouseButtonEvent &event)
{
    auto window = getWindowWithID(event.windowID);
    if(!window)
        return;

    auto morphicEvent = std::make_shared<MouseButtonUpEvent> ();
    morphicEvent->position = Vector2(event.x, event.y);
    morphicEvent->buttonIndex = event.button;

    window->processEvent(morphicEvent);
}

static void onMouseMotion(const SDL_MouseMotionEvent &event)
{
    auto window = getWindowWithID(event.windowID);
    if(!window)
        return;

    auto morphicEvent = std::make_shared<MouseMotionEvent> ();
    morphicEvent->buttonState = event.state;
    morphicEvent->position = Vector2(event.x, event.y);
    morphicEvent->delta = Vector2(event.xrel, event.yrel);

    window->processEvent(morphicEvent);
}

static void onMouseWheel(const SDL_MouseWheelEvent &event)
{
    auto window = getWindowWithID(event.windowID);
    if(!window)
        return;

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    auto morphicEvent = std::make_shared<MouseWheelEvent> ();
    morphicEvent->position = Vector2(mouseX, mouseY);
    morphicEvent->scrollAmount = Vector2(event.x, event.y);
    window->processEvent(morphicEvent);
}

static void onWindowEvent(const SDL_WindowEvent &event)
{
    auto window = getWindowWithID(event.windowID);
    if(!window)
        return;
    
    switch(event.event)
    {
    case SDL_WINDOWEVENT_RESIZED:
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        window->onSizeChanged();
        break;

    case SDL_WINDOWEVENT_CLOSE:
        window->onCloseRequest();
        break;
    default:
        break;
    }
}

void processEvent(const SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_KEYDOWN:
        onKeyboardDown(event->key);
        break;
    case SDL_KEYUP:
        onKeyboardUp(event->key);
        break;
    case SDL_TEXTINPUT:
        onTextInput(event->text);
        break;
    case SDL_MOUSEBUTTONDOWN:
        onMouseButtonDown(event->button);
        break;
    case SDL_MOUSEBUTTONUP:
        onMouseButtonUp(event->button);
        break;
    case SDL_MOUSEMOTION:
        onMouseMotion(event->motion);
        break;
    case SDL_MOUSEWHEEL:
        onMouseWheel(event->wheel);
        break;
    case SDL_WINDOWEVENT:
        onWindowEvent(event->window);
        break;
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
    auto lastTime = Utility::getCurrentMilliseconds();
    while(sdlWindowMap.size() > 0 && !isQuitting)
    {
        processEvents();

        auto newTime = Utility::getCurrentMilliseconds();
        auto deltaTime = (newTime - lastTime)*0.001;
        for(auto &handleAndWindow : sdlWindowMap)
        {
            handleAndWindow.second->updateAndRender(deltaTime);
        }

        lastTime = newTime;
    }
    return 0;
}

SystemWindowPtr Morph::openInSystemWindow()
{
    auto systemWindow = std::make_shared<SystemWindow> ();
    systemWindow->bounds = Rectangle(Vector2(0, 0), bounds.extent());
    systemWindow->addMorph(shared_from_this());

    auto layout = std::make_shared<FillMorphicLayout> ();
    layout->morph = shared_from_this();
    systemWindow->setLayout(layout);
    systemWindow->open();

    return systemWindow;
}

SystemWindow::SystemWindow()
{
    color = Vector4(1, 1, 1, 1);
}

void SystemWindow::open()
{
    if(handle)
        return;

    ensureSDLInitialization();

    auto extent = bounds.extent();
    windowPixelSize = extent;
    handle = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, int(windowPixelSize.x), int(windowPixelSize.y),
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    auto windowId = SDL_GetWindowID(handle);
    sdlWindowMap.insert(std::make_pair(windowId, std::static_pointer_cast<SystemWindow> (shared_from_this())));

    // Get the window info.
    SDL_SysWMinfo windowInfo;
    SDL_VERSION(&windowInfo.version);
    SDL_GetWindowWMInfo(handle, &windowInfo);

    memset(&currentSwapChainCreateInfo, 0, sizeof(currentSwapChainCreateInfo));
    switch(windowInfo.subsystem)
    {
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
    case SDL_SYSWM_WINDOWS:
        currentSwapChainCreateInfo.window = (agpu_pointer)windowInfo.info.win.window;
        break;
#endif
#if defined(SDL_VIDEO_DRIVER_X11)
    case SDL_SYSWM_X11:
        currentSwapChainCreateInfo.window = (agpu_pointer)(uintptr_t)windowInfo.info.x11.window;
        break;
#endif
#if defined(SDL_VIDEO_DRIVER_COCOA)
    case SDL_SYSWM_COCOA:
        currentSwapChainCreateInfo.window = (agpu_pointer)windowInfo.info.cocoa.window;
        break;
#endif
    default:
        fprintf(stderr, "Unsupported window system\n");
        return;
    }

    currentSwapChainCreateInfo.colorbuffer_format = Rendering::RenderingContext::WindowColorBufferFormat;
    currentSwapChainCreateInfo.width  = agpu_uint(windowPixelSize.x);
    currentSwapChainCreateInfo.height = agpu_uint(windowPixelSize.y);
    currentSwapChainCreateInfo.buffer_count = 3;
    currentSwapChainCreateInfo.flags = AGPU_SWAP_CHAIN_FLAG_APPLY_SCALE_FACTOR_FOR_HI_DPI;
    auto renderingContext = Woden::Rendering::RenderingContext::getMainContext();
    if (renderingContext->vsyncDisabled)
    {
        currentSwapChainCreateInfo.presentation_mode = AGPU_SWAP_CHAIN_PRESENTATION_MODE_MAILBOX;
        currentSwapChainCreateInfo.fallback_presentation_mode = AGPU_SWAP_CHAIN_PRESENTATION_MODE_IMMEDIATE;
    }

    auto device = renderingContext->device;
    auto commandQueue = renderingContext->defaultCommandQueue;

    swapChain = device->createSwapChain(commandQueue, &currentSwapChainCreateInfo);
    if(!swapChain)
    {
        fprintf(stderr, "Failed to open the swap chain\n");
        abort();
    }

    windowPixelSize = Vector2(swapChain->getWidth(), swapChain->getHeight());
    int windowWidth, windowHeight;
    SDL_GetWindowSize(handle, &windowWidth, &windowHeight);
    setExtent(Math::Vector2(windowWidth, windowHeight));

    commandAllocator = device->createCommandAllocator(AGPU_COMMAND_LIST_TYPE_DIRECT, commandQueue);
    commandList = device->createCommandList(AGPU_COMMAND_LIST_TYPE_DIRECT, commandAllocator, nullptr);
    commandList->close();

    guiRenderer = std::make_shared<Woden::Rendering::GUIRenderer> ();
    if(!guiRenderer->initialize())
    {
        fprintf(stderr, "Failed to initialize the GUI renderer\n");
        abort();
    }
}

void SystemWindow::close()
{
    if(!handle)
        return;

    auto renderingContext = Woden::Rendering::RenderingContext::getMainContext();
    renderingContext->device->finishExecution();

    auto windowId = SDL_GetWindowID(handle);
    auto it = sdlWindowMap.find(windowId);
    if(it != sdlWindowMap.end())
        sdlWindowMap.erase(it);

    SDL_DestroyWindow(handle);
    handle = nullptr;
}

void SystemWindow::fullDrawWith(const Rendering::GUIRendererPtr &renderer)
{
    drawWith(renderer);
    drawChildrenWith(renderer);
}

void SystemWindow::updateAndRender(Scalar deltaTime)
{
    // GUI updating
    fullUpdate(deltaTime);

    // Rendering
    auto renderingContext = Woden::Rendering::RenderingContext::getMainContext();
    commandAllocator->reset();
    commandList->reset(commandAllocator, nullptr);

    auto scaleFactor = windowPixelSize / getExtent();

    // GUI rendering
    guiRenderer->reset();
    guiRenderer->framebufferExtent = windowPixelSize;
    guiRenderer->scaleFactor = scaleFactor;
    guiRenderer->renderingCommandList = commandList;
    fullDrawWith(guiRenderer);
    guiRenderer->uploadDataWithCommandList(commandList);
    
    // Window rendering
    auto backBuffer = swapChain->getCurrentBackBuffer();

    // Begin the rendering pass.
    commandList->beginRenderPass(renderingContext->windowRenderPass, backBuffer, false);

    commandList->setViewport(0, 0, windowPixelSize.x, windowPixelSize.y);
    commandList->setScissor(0, 0, windowPixelSize.x, windowPixelSize.y);

    guiRenderer->drawOnCommandList(commandList);

    // Finish the command list
    commandList->endRenderPass();
    commandList->close();

    // Swap the buffers.
    renderingContext->defaultCommandQueue->addCommandList(commandList);
    swapBuffers();
    
    renderingContext->device->finishExecution();
}

void SystemWindow::swapBuffers()
{
    try
    {
        swapChain->swapBuffers();
    }
    catch(agpu_exception &e)
    {
        auto errorCode = e.getErrorCode();
        if(errorCode == AGPU_OUT_OF_DATE || errorCode == AGPU_SUBOPTIMAL)
        {
            // We must recreate the swap chain.
            recreateSwapChain();
        }
        else
        {
            throw e;
        }
    }
}

void SystemWindow::onCloseRequest()
{
    printf("TODO: On close request\n");
}

void SystemWindow::onSizeChanged()
{
    recreateSwapChain();
}

void SystemWindow::recreateSwapChain()
{
    auto renderingContext = Woden::Rendering::RenderingContext::getMainContext();
    int w, h;
    SDL_GetWindowSize(handle, &w, &h);

    renderingContext->device->finishExecution();
    auto newSwapChainCreateInfo = currentSwapChainCreateInfo;
    newSwapChainCreateInfo.width = w;
    newSwapChainCreateInfo.height = h;
    newSwapChainCreateInfo.old_swap_chain = swapChain.get();
    swapChain = renderingContext->device->createSwapChain(renderingContext->defaultCommandQueue, &newSwapChainCreateInfo);

    windowPixelSize = Vector2(swapChain->getWidth(), swapChain->getHeight());
    bounds = Rectangle(Vector2(0, 0), Vector2(w, h));
    updateLayout();
}

} // End of namespace Morphic
} // End of namespace Woden