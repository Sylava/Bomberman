#pragma once

#include <SDL2/SDL.h>
#include "Bomb.h"

class Bomb;

enum class ETileType
{
    EMPTY,
    BREAKABLE,
    PILAR,
};

struct STile
{
    SDL_Point position;
    ETileType tileType;
    SDL_Rect hitBox;
    Bomb* bomb = nullptr;

    STile(int x, int y, ETileType inTileType);
    STile() {}

    STile* GetRight() const;
    STile* GetLeft() const;
    STile* GetUp() const;
    STile* GetDown() const;
};