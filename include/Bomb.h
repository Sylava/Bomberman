#pragma once

#include <SDL2/SDL.h>
#include "Tile.h"
#include "renderer.h"
#include "TickableObject.h"
#include <functional>

class Bomb : public TickableObject
{
public:
    SDL_Point position;

    Bomb(SDL_Texture* inBombTex, SDL_Point const& inPosition, std::function<void()> inOnExplodes);

    virtual void tick(float deltaTime) override;
    void print(SDL_Point const& position) const;

private:
    std::function<void()> onExplodes;
    SDL_Texture* bombTex;
    float lifeTime = 2.f;
    float animTime = 0.2f;
    int animIndex = 0;
    bool animDirection = true;
    
    void explodes();
};