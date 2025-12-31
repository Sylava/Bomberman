#pragma once

#include <vector>
#include "Renderer.h"

class Inputs;
class Board;
class Player;
class TickableObject;

class Bomberman
{
public:
    Bomberman(Renderer* inRenderer);
    ~Bomberman();

    void run();
    static Board* getBoard();
    static Inputs* getInputs();
    static Renderer* getRenderer();
    static void registerObject(TickableObject* object);
    static void unregisterObject(TickableObject* object);

private:
    static Bomberman* instance;
    Inputs* input = nullptr;
    Board* board = nullptr;
    Renderer* renderer = nullptr;
    Player* player = nullptr;
    std::vector<TickableObject*> tickableObjects;
};