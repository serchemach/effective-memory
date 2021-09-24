#pragma once
#include "SDL2/SDL.h"
#define BASE_BRIGHTNESS 200.0f

struct Button{
    SDL_Rect rect;
    int isActive;
    int isPressed;
    int isTriggering;
    char* text;
    float brightness;
    SDL_Texture* textTexture;
    SDL_Rect textRect;
};
typedef struct Button Button;

Button CreateButton(SDL_Renderer* renderer, int xPos, int yPos, int width, int height, char* text);
void RenderButton(SDL_Renderer* renderer, Button* button);
void UpdateButton(Button* button, int mousePosX, int mousePoxY, int mouseLeftDown, int mouseLeftUp);
