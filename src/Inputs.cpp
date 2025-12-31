#include "inputs.h"
#include <iostream>
#include <algorithm>

void Inputs::update()
{
    SDL_Event events[32];
    SDL_PumpEvents();
    int num = SDL_PeepEvents(events, 32, SDL_GETEVENT, SDL_KEYDOWN, SDL_KEYUP);
    pressedKeys.clear();
    upKeys.clear();
    for(int i = 0; i < num; ++i)
    {
        if(events[i].type == SDL_KEYDOWN && !events[i].key.repeat)
        {
            pressedKeys.push_back(events[i].key.keysym.sym);
            holdKeys.push_back(events[i].key.keysym.sym);
        }
        else if(events[i].type == SDL_KEYUP)
        {
            upKeys.push_back(events[i].key.keysym.sym);
            auto pos = std::find(holdKeys.begin(), holdKeys.end(), events[i].key.keysym.sym);
            holdKeys.erase(pos);
        }
    }
}

bool Inputs::hasKeyDown(SDL_Keycode key)
{
    return std::find(pressedKeys.begin(), pressedKeys.end(), key) != pressedKeys.end();
}

bool Inputs::hasKeyUp(SDL_Keycode key)
{
    return std::find(upKeys.begin(), upKeys.end(), key) != upKeys.end();
}

bool Inputs::hasKeyHold(SDL_Keycode key)
{
    return std::find(holdKeys.begin(), holdKeys.end(), key) != holdKeys.end();
}