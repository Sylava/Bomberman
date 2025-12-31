#include "Helper/ImageLoader.h"

SDL_Texture* ImageLoader::loadImage(Renderer* renderer, std::string const& path, SDL_Rect const& rect)
{
    if(renderer == nullptr)
        return nullptr;
    // on charge l'image dans une surface
    SDL_Surface* src_surface = SDL_LoadBMP(path.c_str());
    if(src_surface == nullptr)
        return nullptr;
    // on créer une seconde surface aux dimensions qui nous intéresse
    SDL_Surface* target_surface = SDL_CreateRGBSurfaceWithFormat(0, rect.w, rect.h, 32, SDL_PIXELFORMAT_RGBA32);
    // on copie la partie de la surface source qu'on souhaite garder dans la surface destination
    SDL_BlitSurface(src_surface, &rect, target_surface, NULL);
    // on créer une texture depuis la seconde surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer->Get(), target_surface);
    // on free les surfaces qui ne servent plus à rien
    SDL_FreeSurface(src_surface);
    SDL_FreeSurface(target_surface);
    return texture;
}