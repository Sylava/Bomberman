#pragma once

#include <SDL2/SDL.h>
#include "Tile.h"
#include "renderer.h"
#include "TickableObject.h"
#include <functional>
#include <vector>
#include "ExplosionCell.h"

class Bomb : public TickableObject
{
public:
    SDL_Point position;

    Bomb(SDL_Texture* inBombTex, SDL_Point const& inPosition, int power, std::function<void()> inOnExplodes);

    virtual void tick(float deltaTime) override;
    void print(SDL_Point const& position) const;
    void explodes();
    bool isExploding() const { return exploding; }

private:
    std::function<void()> onExplodes;
    SDL_Texture* bombTex;
    float lifeTime = 2.f;
    float animTime = 0.15f;
    int animIndex = 0;
    int range = 1;
    bool animDirection = true;
    std::vector<ExplosionCell> explodingArea;
    int exploIndex = 0;
    float exploTime = 0.1f;
    bool exploding = false;
    bool exploReverseAnim = false;
    
    void destroyBomb();
};