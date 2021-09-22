#pragma once
#include "SDL2/SDL.h"
#define BG_BRIGHTNESS 70


void startRender();
void InitialiseRender(int xres, int yres, SDL_Window** win, SDL_Renderer** renderer);
void ExitRender(SDL_Window* win);
