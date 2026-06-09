#include "Bonus.h"
#include <vector>
#include "renderer.h"
#include "Bomberman.h"
#include "Board.h"
#include <iostream>

Bonus::Bonus(SDL_Texture* inBonusTex, SDL_Point inPosition) : TickableObject()
{
    position = inPosition;
    bonusTex = inBonusTex;
    hitbox = {inPosition.x * 16 + 16, inPosition.y * 16 + 8, 16, 16};
    type = getRandomType();
    switch (type)
    {
        case EBonusType::BOMBUP:
            animType = 0;
            break;
        case EBonusType::POWERUP:
            animType = 2;
            break;
        case EBonusType::SPEEDUP:
            animType = 4;
            break;
        case EBonusType::MALUS:
            animType = 9;
            break;
    }
}

EBonusType Bonus::getRandomType()
{
    std::vector<EBonusType> pool = {
        EBonusType::BOMBUP,
        EBonusType::BOMBUP,
        EBonusType::BOMBUP,
        EBonusType::POWERUP,
        EBonusType::POWERUP,
        EBonusType::POWERUP,
        EBonusType::SPEEDUP,
        EBonusType::SPEEDUP,
        //EBonusType::MALUS,
    };

    int b = rand() % pool.size();
    return pool[b];
}

void Bonus::tick(float deltaTime)
{
    animTime -= deltaTime;
    if(animTime <= 0)
    {
        animTime = 0.15f;
        ++animIndex;
        if(animIndex >= 11)
            animIndex = 0;
    }
    if(destroying && animIndex >= 4)
    {
        Board* board = Bomberman::getBoard();
        board->removeBonus(this);
    }
}

void Bonus::destroyed()
{
    destroying = true;
    animIndex = 0;
    animType = 10;
    animTime = 0.2f;
}

void Bonus::print()
{
    Renderer* renderer = Bomberman::getRenderer();
    SDL_SetRenderTarget(renderer->Get(), NULL);
    SDL_Rect src = {animIndex * 16, animType * 16, 16, 16};
    SDL_Rect dst = {position.x * 16 + 16, position.y * 16 + 8, 16, 16};
    SDL_RenderCopy(renderer->Get(), bonusTex, &src, &dst);
}