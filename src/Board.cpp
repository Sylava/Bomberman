#include "board.h"
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include "Helper/ImageLoader.h"
#include <cmath>
#include <iostream>
#include "Bomberman.h"
#include <algorithm>

Board::Board(int width, int height)
{
    this->width = width;
    this->height = height;
    Renderer* renderer = Bomberman::getRenderer();

    CreateBoard();
    bombTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {0, 0, 48, 16}));
    backgroundTex = ImageLoader::loadImage(renderer, "images/Background.bmp", {0, 0, 240, 160});
    blockTex = ImageLoader::loadImage(renderer, "images/Background.bmp", {495, 335, 16, 16});
    destroyedBlockTex = ImageLoader::loadImage(renderer, "images/standard_map.bmp", {16, 160, 80, 16});
    explosionCenterTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {0, 16, 64, 16}));
    explosionDownTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {0, 32, 64, 16}));
    explosionEndDownTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {64, 32, 64, 16}));
    explosionUpTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {0, 48, 64, 16}));
    explosionEndUpTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {64, 48, 64, 16}));
    explosionLeftTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {0, 64, 64, 16}));
    explosionEndLeftTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {64, 64, 64, 16}));
    explosionRightTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {0, 80, 64, 16}));
    explosionEndRightTex = (ImageLoader::loadImage(renderer, "images/Items.bmp", {64, 80, 64, 16}));
}

Board::~Board()
{
    SDL_DestroyTexture(backgroundTex);
    SDL_DestroyTexture(bombTex);
    SDL_DestroyTexture(blockTex);
    DestroyBoard();
}

void Board::CreateBoard()
{
    std::array<SDL_Point, 12> corners_tab = {{
        {0,0}, {1,0}, {11,0}, {12,0},
        {0,1}, {12,1},
        {0,7}, {12,7},
        {0,8}, {1,8}, {11,8}, {12,8}
    }};
    
    tiles = new STile*[height];
    for(int y = 0; y < height; ++y)
    {
        // attention ça crée n tile via son constructeur STile()
        tiles[y] = new STile[width]();
        // ... donc on modifie chaque tile créée
        for(int x = 0; x < width; ++x)
        {
            tiles[y][x].tileType = ETileType::EMPTY;
            if(IsPilarTile(x, y))
                tiles[y][x].tileType = ETileType::PILAR;
            else if(!IsCorner(x, y, corners_tab) && (rand() % 10) < 8)
                tiles[y][x].tileType = ETileType::BREAKABLE;
            tiles[y][x].position = {x, y};
            tiles[y][x].hitBox = {x * 16 + 16, y * 16 + 8, 16, 16};
        }
    }
}

void Board::DestroyBoard()
{
    for(int i = 0; i < height; ++i)
    {
        delete[] tiles[i];
    }
    delete[] tiles;
}

void Board::tick(float deltaTime)
{
    for(STile* tile : breakingBlocks)
    {
        tile->breakingTime -= deltaTime;
        if(tile->breakingTime <= 0)
        {
            tile->breakingAnim += 1;
            tile->breakingTime = 0.1f;
            if(tile->breakingAnim >= 5)
            {
                tile->breaking = false;
                tile->tileType = ETileType::EMPTY;
                auto pos = std::find(breakingBlocks.begin(), breakingBlocks.end(), tile);
                breakingBlocks.erase(pos);
            }
        }
    }
}

STile& Board::getTileAtPosition(Vector2D const& position) const
{
    int ix = std::round(position.x + 8.f);
    int iy = std::round(position.y + 12.f);
    ix = ix / 16;
    iy = iy / 16;
    return tiles[iy][ix];
}

bool Board::IsPilarTile(int x, int y)
{
    return (x + 1) % 2 == 0 && (y + 1) % 2 == 0;
}

bool Board::IsCorner(int x, int y, const std::array<SDL_Point, 12>& tab)
{
    for(int i = 0; i < 12; ++i)
    {
        if(x == tab[i].x && y == tab[i].y)
            return true;
    }
    return false;
}

void Board::Print() const
{
    Renderer* renderer = Bomberman::getRenderer();
    // au cas où, on force le renderer à dessiner sur l'écran
    SDL_SetRenderTarget(renderer->Get(), NULL);
    SDL_RenderCopy(renderer->Get(), backgroundTex, NULL, NULL);
    for(int y = 0; y < 9; ++y)
    {
        for(int x = 0; x < 13; ++x)
        {
            if(tiles[y][x].tileType == ETileType::BREAKABLE)
            {
                SDL_Rect dst = {tiles[y][x].position.x * 16 + 16, tiles[y][x].position.y * 16 + 8, 16, 16};
                if(tiles[y][x].breaking == false)
                {
                    SDL_RenderCopy(renderer->Get(), blockTex, NULL, &dst);
                }
                else
                {
                    SDL_Rect src = {tiles[y][x].breakingAnim * 16, 0, 16, 16};
                    SDL_RenderCopy(renderer->Get(), destroyedBlockTex, &src, &dst);
                }
            }
            else if(tiles[y][x].bomb != nullptr)
            {
                tiles[y][x].bomb->print(tiles[y][x].position);
            }
        }
    }
}

Vector2D Board::clampPosition(Vector2D const& position) const
{
    Vector2D newPos = position;
    if(newPos.x < 0.f)
        newPos.x = 0.f;
    else if(newPos.x > 12.f * 16.f)
        newPos.x = 12.f * 16.f;
    if(newPos.y < 0.f)
        newPos.y = 0.f;
    else if(newPos.y > 8.f * 16.f)
        newPos.y = 8.f * 16.f;
    return newPos;
}

bool Board::doesPlayerCollides(Vector2D const& position) const
{
    SDL_Rect playerHitbox = {0, 0, 16, 16};
    playerHitbox.x = std::floor(position.x + 16);
    playerHitbox.y = std::floor(position.y + 8);
    for(int y = 0; y < 9; ++y)
    {
        for(int x = 0; x < 13; ++x)
        {
            if(tiles[y][x].tileType != ETileType::EMPTY)
            {
                if(SDL_HasIntersection(&playerHitbox, &tiles[y][x].hitBox))
                    return true;
            }
        }
    }
    return false;
}

bool Board::spawnBomb(Player& player)
{
    STile& tile = getTileAtPosition(player.position);
    if(tile.bomb == nullptr)
    {
        Bomb* bomb = new Bomb(bombTex, tile.position, [&]() -> void
        {
            player.giveBomb();
        });
        tile.bomb = bomb;
        bombs.push_back(bomb);
        return true;
    }
    return false;
}

void Board::removeBomb(Bomb* bomb)
{
    tiles[bomb->position.y][bomb->position.x].bomb = nullptr;
    auto pos = std::find(bombs.begin(), bombs.end(), bomb);
    bombs.erase(pos);
    delete bomb;
}

void Board::getExplodingArea(std::vector<ExplosionCell>& explodingArea, int range, SDL_Point& position)
{
    STile* centerTile = &tiles[position.y][position.x];
    explodingArea.push_back({centerTile, explosionCenterTex});

    STile* checkDownTile = centerTile->GetDown();
    STile* checkUpTile = centerTile->GetUp();
    STile* checkLeftTile = centerTile->GetLeft();
    STile* checkRightTile = centerTile->GetRight();
    // on vérifie les cases de tous les côtés
    for(int i = 0; i < range; ++i)
    {
        if(checkDownTile != nullptr && checkDownTile->tileType == ETileType::EMPTY)
        {
            if(!checkDownTile->bomb)
            {
                if(i < range - 1)
                    explodingArea.push_back({checkDownTile, explosionDownTex});
                else
                    explodingArea.push_back({checkDownTile, explosionEndDownTex});
                checkDownTile = checkDownTile->GetDown();
            }
            else
            {
                if(!checkDownTile->bomb->isExploding())
                    checkDownTile->bomb->detonate();
                checkDownTile = nullptr;
            }
        }
        else if(checkDownTile != nullptr && checkDownTile->tileType == ETileType::BREAKABLE)
        {
            checkDownTile->breaking = true;
            breakingBlocks.push_back(checkDownTile);
            checkDownTile = nullptr;
        }

        if(checkUpTile != nullptr && checkUpTile->tileType == ETileType::EMPTY)
        {
            if(!checkUpTile->bomb)
            {
                if(i < range - 1)
                    explodingArea.push_back({checkUpTile, explosionUpTex});
                else
                    explodingArea.push_back({checkUpTile, explosionEndUpTex});
                checkUpTile = checkUpTile->GetUp();
            }
            else
            {
                if(!checkUpTile->bomb->isExploding())
                    checkUpTile->bomb->detonate();
                checkUpTile = nullptr;
            }
        }
        else if(checkUpTile != nullptr && checkUpTile->tileType == ETileType::BREAKABLE)
        {
            checkUpTile->breaking = true;
            breakingBlocks.push_back(checkUpTile);
            checkUpTile = nullptr;
        }

        if(checkLeftTile != nullptr && checkLeftTile->tileType == ETileType::EMPTY)
        {
            if(!checkLeftTile->bomb)
            {
                if(i < range - 1)
                    explodingArea.push_back({checkLeftTile, explosionLeftTex});
                else
                    explodingArea.push_back({checkLeftTile, explosionEndLeftTex});
                checkLeftTile = checkLeftTile->GetLeft();
            }
            else
            {
                if(!checkLeftTile->bomb->isExploding())
                    checkLeftTile->bomb->detonate();
                checkLeftTile = nullptr;
            }
        }
        else if(checkLeftTile != nullptr && checkLeftTile->tileType == ETileType::BREAKABLE)
        {
            checkLeftTile->breaking = true;
            breakingBlocks.push_back(checkLeftTile);
            checkLeftTile = nullptr;
        }

        if(checkRightTile != nullptr && checkRightTile->tileType == ETileType::EMPTY)
        {
            if(!checkRightTile->bomb)
            {
                if(i < range - 1)
                    explodingArea.push_back({checkRightTile, explosionRightTex});
                else
                    explodingArea.push_back({checkRightTile, explosionEndRightTex});
                checkRightTile = checkRightTile->GetRight();
            }
            else
            {
                if(!checkRightTile->bomb->isExploding())
                    checkRightTile->bomb->detonate();
                checkRightTile = nullptr;
            }
        }
        else if(checkRightTile != nullptr && checkRightTile->tileType == ETileType::BREAKABLE)
        {
            checkRightTile->breaking = true;
            breakingBlocks.push_back(checkRightTile);
            checkRightTile = nullptr;
        }
    }
}

STile* Board::GetRight(STile const& tile)
{
    if(tile.position.x >= width - 1)
        return nullptr;
    return &tiles[tile.position.y][tile.position.x + 1];
}
STile* Board::GetLeft(STile const& tile)
{
    if(tile.position.x <= 0)
        return nullptr;
    return &tiles[tile.position.y][tile.position.x - 1];
}
STile* Board::GetUp(STile const& tile)
{
    if(tile.position.y <= 0)
        return nullptr;
    return &tiles[tile.position.y - 1][tile.position.x];
}
STile* Board::GetDown(STile const& tile)
{
    if(tile.position.y >= height - 1)
        return nullptr;
    return &tiles[tile.position.y + 1][tile.position.x];
}