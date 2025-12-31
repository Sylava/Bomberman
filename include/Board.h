#pragma once

#include <SDL2/SDL.h>
#include "renderer.h"
#include "tile.h"
#include <array>
#include <vector>
#include "Math/Vector2D.h"
#include "bomb.h"
#include "player.h"

class Board
{
public:
    Board(int width, int height);
    ~Board();

    STile* GetRight(STile const& tile);
    STile* GetLeft(STile const& tile);
    STile* GetUp(STile const& tile);
    STile* GetDown(STile const& tile);
    Vector2D clampPosition(Vector2D const& position) const;
    bool doesPlayerCollides(Vector2D const& position) const;
    STile& getTileAtPosition(Vector2D const& position) const;
    bool spawnBomb(Player& player);
    void removeBomb(Bomb* bomb);

    void Print() const;

private:
    SDL_Texture* bombTex;
    SDL_Texture* backgroundTex;
    SDL_Texture* blockTex;
    STile** tiles;
    int width, height;
    std::vector<Bomb*> bombs;

    void CreateBoard();
    void DestroyBoard();
    bool IsPilarTile(int x, int y);
    bool IsCorner(int x, int y, const std::array<SDL_Point, 12>& tab);
};
