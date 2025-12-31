#pragma once

#include <SDL2/SDL.h>
#include <string>
#include "renderer.h"

class ImageLoader
{
public:
    static SDL_Texture* loadImage(Renderer* renderer, std::string const& path, SDL_Rect const& rect);
};