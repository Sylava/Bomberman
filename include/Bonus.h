#pragma once

#include <SDL2/SDL.h>
#include "TickableObject.h"

enum class EBonusType
{
    BOMBUP,
    POWERUP,
    SPEEDUP,
    MALUS,
};

class Bonus : public TickableObject
{
public:
    Bonus(SDL_Texture* inBonusTex, SDL_Point inPosition);
    virtual void tick(float deltaTime) override;
    void destroyed();
    void print();

    SDL_Point position;
    SDL_Rect hitbox;
    EBonusType type;
    bool destroying = false;

private:
    SDL_Texture* bonusTex;
    int animType;
    int animIndex = 0;
    float animTime = 0.2f;
    EBonusType getRandomType();
};