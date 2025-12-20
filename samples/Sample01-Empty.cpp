#include "Woden/Morphic/Morph.hpp"

using namespace Woden::Morphic;

/*
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <stdio.h>

int screenWidth = 1024;
int screenHeight = 576;
SDL_Window *window;
SDL_Renderer *windowRenderer;
bool isQuitting = false;

void processEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE)
                isQuitting = true;

        case SDL_QUIT:
            isQuitting = true;
            break;
        }
    }
}

int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);

    window = SDL_CreateWindow("Sample01-Empty", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if(!window)
    {
        fprintf(stderr, "Failed to create SDL2 window");
        return 1;
    }

    windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(!windowRenderer)
    {
        fprintf(stderr, "Failed to create the SDL2 window renderer.");
        return 1;
    }

    while(!isQuitting)
    {
        processEvents();

        SDL_SetRenderDrawColor(windowRenderer, 0, 0, 255, 255);
        SDL_RenderClear(windowRenderer);
        SDL_RenderPresent(windowRenderer);
    }

    SDL_DestroyRenderer(windowRenderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
*/

int main(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
    
    auto morph = std::make_shared<Morph> ();
    morph->openInSystemWindow();
    return Morph::runMainLoop();
}