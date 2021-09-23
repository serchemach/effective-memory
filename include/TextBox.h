#pragma once
#include "SDL2/SDL.h"
#define TBOX_BASE_BRIGHTNESS 79
#define TBOX_TEXT_LENGTH 50

struct TextBox{
    SDL_Rect rect;
    float brightness;
    int isActive;
    int isUsed;
    char* text;
    int curTextSize;
    SDL_Texture* textTexture;
    SDL_Rect textDRect;
    SDL_Rect textFRect;
};
typedef struct TextBox TextBox;

TextBox CreateTextBox(SDL_Renderer* renderer, int xPos, int yPos, int width, int height, char* text);
void RenderTextBox(SDL_Renderer* renderer, TextBox tBox);
void UpdateTextBox(SDL_Renderer* renderer, TextBox* tBox, int mousePosX, int mousePosY, int mouseLeftDown, char lastChar, int backspacePressed);
void UpdateTextBoxTextTexture(SDL_Renderer* renderer, TextBox* tBox);
