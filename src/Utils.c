#include "Utils.h"
#include "SDL_TTF/SDL_ttf.h"

SDL_Texture* CreateTextTexture(SDL_Renderer* renderer, int fontSize, char* text)
{    
    SDL_Color textColor = {0, 0, 0};
    TTF_Font* font = TTF_OpenFont("C:/windows/fonts/Calibri.ttf", fontSize);
    SDL_Surface* tSurf = TTF_RenderText_Blended(font, text, textColor);
    return SDL_CreateTextureFromSurface(renderer, tSurf);
}
