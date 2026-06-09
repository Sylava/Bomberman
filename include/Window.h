#pragma once
#include <SDL2/SDL.h>

class Window
{
public:
    Window(int size_x, int size_y);
    ~Window();

    SDL_Window* Get() { return window; }
    SDL_Point GetSize() { return size; }
private:
    SDL_Window* window;
    SDL_Point size;
};