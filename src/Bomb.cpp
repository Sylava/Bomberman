#include "Bomb.h"
#include "Helper/ImageLoader.h"
#include "Bomberman.h"
#include "Board.h"

Bomb::Bomb(SDL_Texture* inBombTex, SDL_Point const& inPosition, std::function<void()> inOnExplodes) : TickableObject()
{
    bombTex = inBombTex;
    position = inPosition;
    onExplodes = inOnExplodes;
}

void Bomb::tick(float deltaTime)
{
    animTime -= deltaTime;
    if(animTime <= 0)
    {
        animTime = 0.2f;
        if(animDirection)
        {
            if(animIndex == 2)
                animDirection = false;
            else
                ++animIndex;
        }
        else
        {
            if(animIndex == 0)
                animDirection = true;
            else
                --animIndex;
        }
    }
    lifeTime -= deltaTime;
    if(lifeTime <= 0)
        explodes();
}

void Bomb::explodes()
{
    onExplodes();
    Board* board = Bomberman::getBoard();
    board->removeBomb(this);
}

void Bomb::print(SDL_Point const& position) const
{
    Renderer* renderer = Bomberman::getRenderer();
    SDL_SetRenderTarget(renderer->Get(), NULL);
    SDL_Rect dst = {position.x * 16 + 16, position.y * 16 + 8, 16, 16};
    SDL_Rect src = {animIndex * 16, 0, 16, 16};
    SDL_RenderCopy(renderer->Get(), bombTex, &src, &dst);
}