#include "player.h"
#include "Helper/ImageLoader.h"
#include "inputs.h"
#include "Math/Vector2D.h"
#include <cmath>
#include <iostream>
#include "Bomb.h"
#include "Bomberman.h"
#include "Board.h"

Player::Player(int nbBomb) : TickableObject()
{
    maxBomb = nbBomb;
    SDL_Rect rect = {0, 0, 16 * 4, 24 * 4};
    playerTexture = ImageLoader::loadImage(Bomberman::getRenderer(), "images/Bomberman.bmp", rect);
    position = {0, 0};
}

Player::~Player()
{
    SDL_DestroyTexture(playerTexture);
}

void Player::tick(float deltaTime)
{
    bool wasMoving = velocity != Vector2D::zero;
    manageMovements(deltaTime);
    if(Bomberman::getInputs()->hasKeyHold(SDLK_SPACE))
    {
        if(maintainBombTimer <= 0.f)
        {
            Board* board = Bomberman::getBoard();
            if(maxBomb > 0 && board->spawnBomb(*this))
            {
                maxBomb -= 1;
                maintainBombTimer = 0.2f;
            }
        }
        maintainBombTimer -= deltaTime;
    }
    else
    {
        maintainBombTimer = 0;
    }

    if(velocity != Vector2D::zero)
    {
        if(wasMoving == false)
        {
            timer = 0;
            animIndex = 1;
        }
        timer += deltaTime;
        if(timer > 0.2f)
        {
            animIndex = (animIndex + 1) % 4;
            timer = 0;
        }
    }
    else
    {
        animIndex = 0;
    }
}

void Player::manageMovements(float deltaTime)
{
    Board* board = Bomberman::getBoard();
    STile& playerTile = board->getTileAtPosition(position);
    Vector2D oldPosition = position;
    float delta = 4.f;
    float eps = 0.5f;
    float modx = fmod(position.x, 16);
    float mody = fmod(position.y, 16);

    velocity = {0.f, 0.f};
    STile* tileToTest = nullptr;
    if(Bomberman::getInputs()->hasKeyHold(SDLK_s))
    {
        velocity = goDown(delta, eps, modx);
        tileToTest = playerTile.GetDown();
    }
    else if(Bomberman::getInputs()->hasKeyHold(SDLK_z))
    {
        velocity = goUp(delta, eps, modx);
        tileToTest = playerTile.GetUp();
    }
    else if(Bomberman::getInputs()->hasKeyHold(SDLK_q))
    {
        velocity = goLeft(delta, eps, mody);
        tileToTest = playerTile.GetLeft();
    }
    else if(Bomberman::getInputs()->hasKeyHold(SDLK_d))
    {
        velocity = goRight(delta, eps, mody);
        tileToTest = playerTile.GetRight();
    }

    Vector2D newPosition = position;
    newPosition += velocity * deltaTime * speed;
    newPosition = board->clampPosition(newPosition);
    
    SDL_Rect playerHitbox = {0, 0, 16, 16};
    playerHitbox.x = std::floor(position.x + 16);
    playerHitbox.y = std::floor(position.y + 8);
    if(tileToTest == nullptr || ((tileToTest->tileType == ETileType::EMPTY || tileToTest->tileType == ETileType::BONUS) && tileToTest->bomb == nullptr) || !SDL_HasIntersection(&playerHitbox, &tileToTest->hitBox))
    {
        position = newPosition;
        velocity = newPosition - oldPosition;
        if(tileToTest != nullptr && tileToTest->tileType == ETileType::BONUS && SDL_HasIntersection(&tileToTest->hitBox, &playerHitbox))
        {
            printf("if null\n");
            if(tileToTest->bonus != nullptr)
            {
                printf("appel remove\n");
                applyBonus(tileToTest->bonus->type);
                board->removeBonus(tileToTest->bonus);
            }
            else
                printf("bonus null\n");
        }
    }
}

void Player::giveBomb()
{
    ++maxBomb;
}

Vector2D Player::goDown(float delta, float eps, float modx)
{
    animNum = 0;
    if(modx < eps || modx > 16.f - eps) // si je suis très proche du centre ou au centre
    {
        position.x = std::roundf(position.x / 16.f) * 16.f; // je centre le perso 
        return {0.f, 1.f}; // je me déplace vers le bas
    }
    else if(fmod(position.x, 16) < delta) // sinon si je suis moins proche du centre et à sa droite
        return {-1.f, 0.f}; // je me déplace vers la gauche
    else if(fmod(position.x, 16) > 16.f - delta) // sinon si je suis moins proche du centre et à sa gauche
        return {1.f, 0.f}; // je me déplace vers la droite
    return {0.f, 0.f};
}

Vector2D Player::goUp(float delta, float eps, float modx)
{
    animNum = 1;
    if(modx < eps || modx > 16.f - eps)
    {
        position.x = std::roundf(position.x / 16.f) * 16.f;
        return {0.f, -1.f};
    }
    else if(fmod(position.x, 16) < delta)
        return {-1.f, 0.f};
    else if(fmod(position.x, 16) > 16.f - delta)
        return {1.f, 0.f};
    return {0.f, 0.f};
}

Vector2D Player::goLeft(float delta, float eps, float mody)
{
    animNum = 2;
    if(mody < eps || mody > 16.f - eps)
    {
        position.y = std::roundf(position.y / 16.f) * 16.f;
        return {-1.f, 0.f};
    }
    else if(fmod(position.y, 16) < delta)
        return {0.f, -1.f};
    else if(fmod(position.y, 16) > 16.f - delta)
        return {0.f, 1.f};
    return {0.f, 0.f};
}

Vector2D Player::goRight(float delta, float eps, float mody)
{
    animNum = 3;
    if(mody < eps || mody > 16.f - eps)
    {
        position.y = std::roundf(position.y / 16.f) * 16.f;
        return {1.f, 0.f};
    }
    else if(fmod(position.y, 16) < delta)
        return {0.f, -1.f};
    else if(fmod(position.y, 16) > 16.f - delta)
        return {0.f, 1.f};
    return {0.f, 0.f};
}

void Player::roundPosition(Vector2D& position) const
{
    float eps = 3.f;
    if(fmod(position.x, 16) < eps || fmod(position.x, 16) > 16.f - eps)
        position.x = std::roundf(position.x / 16.f) * 16.f;
    if(fmod(position.y, 16) < eps || fmod(position.y, 16) > 16.f - eps)
        position.y = std::roundf(position.y / 16.f) * 16.f;
}

void Player::applyBonus(EBonusType& type)
{
    switch (type)
    {
        case EBonusType::BOMBUP:
            maxBomb += 1;
            break;
        case EBonusType::POWERUP:
            power += 1;
            break;
        case EBonusType::SPEEDUP:
            speed += 6;
            break;
        case EBonusType::MALUS:
            break;
    }
}

void Player::Print() const
{
    Renderer* renderer = Bomberman::getRenderer();
    SDL_Rect src = {16 * animIndex, 24 * animNum, 16, 24};
    SDL_Rect dest = {(int)position.x + 16, (int)position.y, 16, 24};
    SDL_SetRenderTarget(renderer->Get(), NULL);
    SDL_RenderCopy(renderer->Get(), playerTexture, &src, &dest);
}