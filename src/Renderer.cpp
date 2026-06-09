#include "renderer.h"

Renderer::Renderer(Window& window)
{
    renderer = SDL_CreateRenderer(window.Get(), -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, 240, 160);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
}

void Renderer::Clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::Print()
{
    SDL_RenderPresent(renderer);
}