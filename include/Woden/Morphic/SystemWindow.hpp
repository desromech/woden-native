#ifndef WODEN_MORPHIC_SYSTEM_WINDOW_HPP
#define WODEN_MORPHIC_SYSTEM_WINDOW_HPP

#include "Morph.hpp"
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

    SDL_Window *handle = nullptr;
    std::string title;
};


} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_SYSTEM_WINDOW_HPP