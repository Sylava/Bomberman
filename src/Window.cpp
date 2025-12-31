#include "Window.h"

Window::Window(int size_x, int size_y)
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Bomberman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size_x, size_y, SDL_WINDOW_FULLSCREEN);
    size = {size_x, size_y};
}

Window::~Window()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}