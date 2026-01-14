#ifndef WODEN_MORPHIC_SYSTEM_WINDOW_HPP
#define WODEN_MORPHIC_SYSTEM_WINDOW_HPP

#include "RootMorph.hpp"
#include "AGPU/agpu.hpp"

#define SDL_MAIN_HANDLED
#include <SDL.h>

namespace Woden
{
namespace Morphic
{

/**
 * I am a system window morph
 */
class SystemWindow : public RootMorph
{
public:
    SystemWindow();

    void open();
    void close();
    void updateAndRender(Scalar deltaTime);
    void swapBuffers();
    void recreateSwapChain();

    virtual void fullDrawWith(const Rendering::GUIRendererPtr &renderer) override;

    SDL_Window *handle = nullptr;
    std::string title;
    Vector2 windowPixelSize;

    agpu_swap_chain_create_info currentSwapChainCreateInfo;
    agpu_swap_chain_ref swapChain;
    
    agpu_command_allocator_ref commandAllocator;
    agpu_command_list_ref commandList;

    Rendering::GUIRendererPtr guiRenderer;
};


} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_SYSTEM_WINDOW_HPP