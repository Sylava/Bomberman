#pragma once

#include <SDL2/SDL.h>
#include "Tile.h"

struct STile;

struct ExplosionCell {
    STile* tile;
    SDL_Texture* explosionTex;
};