#pragma once
#include <SDL2/SDL.h>
#include "window.h"

class Renderer
{
public:
    Renderer(Window& window);
    ~Renderer();
    void Clear();
    void Print();

    SDL_Renderer* Get() { return renderer; }
private:
    SDL_Renderer* renderer;
};