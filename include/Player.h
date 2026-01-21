#pragma once

#include <SDL2/SDL.h>
#include "Math/Vector2D.h"
#include <vector>
#include "Tile.h"
#include "TickableObject.h"
#include "Bonus.h"

class Player : public TickableObject
{
public:
    Player(int nbBomb);
    virtual ~Player();

    // position du joueur sur l'écran (en pixel) sans son offset
    Vector2D position;
    SDL_Rect hitBox = {0, 0, 16, 16};
    int power = 2;

    void giveBomb();
    virtual void tick(float deltaTime) override;
    void Print() const;

private:
    SDL_Texture* playerTexture;
    Vector2D velocity;
    float speed = 64;
    float timer = 0;
    int animIndex = 0;
    int animNum = 0;
    int maxBomb = 2;
    float maintainBombTimer = 0;

    void manageMovements(float deltaTime);
    Vector2D goDown(float delta, float eps, float modx);
    Vector2D goUp(float delta, float eps, float modx);
    Vector2D goLeft(float delta, float eps, float mody);
    Vector2D goRight(float delta, float eps, float mody);
    void roundPosition(Vector2D& position) const;
    void applyBonus(EBonusType& type);
};