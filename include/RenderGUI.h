#pragma once
#include "SDL2/SDL.h"
#define BG_BRIGHTNESS 75


void startRender();
void InitialiseRender(int xres, int yres, SDL_Window** rWin, SDL_Renderer** rRenderer, SDL_Surface** rRenderSurface, SDL_Surface** rWindowSurface);
void ExitRender(SDL_Window* win);
