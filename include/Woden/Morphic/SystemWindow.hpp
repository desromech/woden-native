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
    SystemWindow();

    void open();
    void close();
    void updateAndRender(Scalar deltaTime);
    void swapBuffers();
    void recreateSwapChain();

    void processKeyboardEvent(const EventPtr &event);
    virtual void setNewKeyboardFocus(const MorphPtr &newKeyboardFocus) override;

    virtual void setNewMouseFocus(const MorphPtr &newMouseFocus);
    virtual MorphPtr getMouseFocus()const override;

    virtual void fullDrawWith(const Rendering::GUIRendererPtr &renderer) override;

    MorphPtr currentKeyboardFocus;
    MorphPtr currentMouseFocus;

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