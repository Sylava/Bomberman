#pragma once

#include <SDL2/SDL.h>
#include "renderer.h"
#include "tile.h"
#include <array>
#include <vector>
#include "Math/Vector2D.h"
#include "bomb.h"
#include "player.h"
#include "ExplosionCell.h"
#include "TickableObject.h"
#include "Bonus.h"

class Board : public TickableObject
{
public:
    Board(int width, int height);
    ~Board();

    virtual void tick(float deltaTime) override;
    STile& getTileAtPosition(Vector2D const& position) const;
    STile* GetRight(STile const& tile);
    STile* GetLeft(STile const& tile);
    STile* GetUp(STile const& tile);
    STile* GetDown(STile const& tile);
    Vector2D clampPosition(Vector2D const& position) const;
    bool doesPlayerCollides(Vector2D const& position) const;
    bool spawnBomb(Player& player);
    void removeBomb(Bomb* bomb);
    void removeBonus(Bonus* bonus);
    void getExplodingArea(std::vector<ExplosionCell>& explodingArea, int range, SDL_Point& position);

    void Print() const;

private:
    SDL_Texture* bombTex;
    SDL_Texture* backgroundTex;
    SDL_Texture* blockTex;
    SDL_Texture* destroyedBlockTex;
    SDL_Texture* explosionCenterTex;
    SDL_Texture* explosionDownTex;
    SDL_Texture* explosionEndDownTex;
    SDL_Texture* explosionUpTex;
    SDL_Texture* explosionEndUpTex;
    SDL_Texture* explosionLeftTex;
    SDL_Texture* explosionEndLeftTex;
    SDL_Texture* explosionRightTex;
    SDL_Texture* explosionEndRightTex;
    SDL_Texture* bonusTex;
    STile** tiles;
    int width, height;
    std::vector<Bomb*> bombs;
    std::vector<STile*> breakingBlocks;
    std::vector<Bonus*> bonusList;

    void CreateBoard();
    void DestroyBoard();
    bool IsPilarTile(int x, int y);
    bool IsCorner(int x, int y, const std::array<SDL_Point, 12>& tab);
    void spawnBonus(SDL_Point& position);
};
