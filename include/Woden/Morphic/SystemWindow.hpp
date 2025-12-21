#ifndef WODEN_MORPHIC_SYSTEM_WINDOW_HPP
#define WODEN_MORPHIC_SYSTEM_WINDOW_HPP

#include "Morph.hpp"
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
class SystemWindow : public Morph
{
public:
    void open();
    void close();
    void updateAndRender();
    void swapBuffers();
    void recreateSwapChain();

    SDL_Window *handle = nullptr;
    std::string title;
    int windowWidth = 640;
    int windowHeight = 480;

    agpu_swap_chain_create_info currentSwapChainCreateInfo;
    agpu_swap_chain_ref swapChain;
    
    agpu_command_allocator_ref commandAllocator;
    agpu_command_list_ref commandList;
};


} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_SYSTEM_WINDOW_HPP