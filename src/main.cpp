#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include "Window.h"
#include "Renderer.h"
#include <vector>
#include <cmath>
#include "Bomberman.h"

int main()
{
    Window window = Window(240, 160);
    Renderer renderer = Renderer(window);
    Bomberman bomberman(&renderer);
    bomberman.run();
    return 0;
}