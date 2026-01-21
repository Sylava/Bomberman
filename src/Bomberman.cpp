#include "Bomberman.h"
#include "Board.h"
#include "Player.h"
#include "Inputs.h"
#include "TickableObject.h"
#include <algorithm>

Bomberman* Bomberman::instance = nullptr;

Bomberman::Bomberman(Renderer* inRenderer)
{
    if(instance == nullptr)
    {
        instance = this;
    }
    renderer = inRenderer;
    input = new Inputs();
    board = new Board(13, 9);
    player = new Player(2);
}

Bomberman::~Bomberman()
{
    delete player;
    delete input;
    delete board;
    if(instance == this)
    {
        instance = nullptr;
    }
}

Board* Bomberman::getBoard()
{
    return instance->board;
}

Inputs* Bomberman::getInputs()
{
    return instance->input;
}

Renderer* Bomberman::getRenderer()
{
    return instance->renderer;
}

void Bomberman::run()
{
    bool run = true;
    Uint32 lastTick = SDL_GetTicks();
    while(run)
    {
        Uint32 totalTime = SDL_GetTicks();
        float deltaTime = (totalTime - lastTick) / 1000.0f; // en secondes
        lastTick = totalTime;

        input->update();
        for(int i = tickableObjects.size() - 1; i >= 0; --i)
        {
            tickableObjects[i]->tick(deltaTime);
        }
        renderer->Clear();
        board->Print();
        player->Print();
        renderer->Print();
        if(input->hasKeyDown(SDLK_ESCAPE))
            run = false;
    }
}


void Bomberman::registerObject(TickableObject* object)
{
    instance->tickableObjects.push_back(object);
}

void Bomberman::unregisterObject(TickableObject* object)
{
    auto pos = std::find(instance->tickableObjects.begin(), instance->tickableObjects.end(), object);
    instance->tickableObjects.erase(pos);
}