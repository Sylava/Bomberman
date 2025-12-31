#include "tile.h"
#include "Bomberman.h"
#include "Board.h"

STile::STile(int x, int y, ETileType inTileType)
{
    position = {x, y};
    tileType  = inTileType;
    hitBox = {x * 16 + 16, y * 16 + 8, 16, 16};
}

STile* STile::GetRight() const { return Bomberman::getBoard()->GetRight(*this);}
STile* STile::GetLeft() const { return Bomberman::getBoard()->GetLeft(*this); }
STile* STile::GetUp() const { return Bomberman::getBoard()->GetUp(*this); }
STile* STile::GetDown() const { return Bomberman::getBoard()->GetDown(*this); }