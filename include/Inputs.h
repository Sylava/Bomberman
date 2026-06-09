#pragma once

#include <SDL2/SDL.h>
#include <vector>

class Inputs
{
public:
    void update();
    bool hasKeyDown(SDL_Keycode key);
    bool hasKeyUp(SDL_Keycode key);
    bool hasKeyHold(SDL_Keycode key);

private:
    std::vector<SDL_Keycode> pressedKeys;
    std::vector<SDL_Keycode> upKeys;
    std::vector<SDL_Keycode> holdKeys;
};