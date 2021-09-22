#include "Button.h"
#include "SDL_TTF/SDL_ttf.h"
#include "stdio.h"

Button CreateButton(SDL_Renderer* renderer, int xPos, int yPos, int width, int height, char* text)
{
    Button result;
    result.rect.h = height; result.rect.w = width;
    result.rect.x = xPos; result.rect.y = yPos;
    
    result.isActive = 0;
    result.isPressed = 0;
    result.text = text;
    result.brightness = BASE_BRIGHTNESS;
    
    SDL_Color textColor = {0, 0, 0};
    TTF_Font* font = TTF_OpenFont("C:/windows/fonts/Calibri.ttf", min(width, height));
    SDL_Surface* tSurf = TTF_RenderText_Blended(font, text, textColor);

    if((float)tSurf->w / width > (float)tSurf->h / height)
    {
        result.textRect.w = width;
        result.textRect.h = (int)((float)(tSurf->h * width) / tSurf->w);
        result.textRect.y = yPos + (height - result.textRect.h) / 2;
        result.textRect.x = xPos + (width - result.textRect.w) / 2;
    }
    else
    {
        result.textRect.h = height;
        result.textRect.w = (int)((float)(tSurf->w * height) / tSurf->h);
        result.textRect.x = xPos + (width - result.textRect.w) / 2;
        result.textRect.y = yPos + (height - result.textRect.h) / 2;
    }

    result.textTexture = SDL_CreateTextureFromSurface(renderer, tSurf);
    return result;
}

void RenderButton(SDL_Renderer* renderer, Button* button)
{
    SDL_SetRenderDrawColor(renderer, (int)button->brightness, (int)button->brightness, (int)button->brightness, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderDrawRect(renderer, &button->rect);

    SDL_RenderCopy(renderer, button->textTexture, NULL, &button->textRect);
}

void UpdateButton(Button* button, int mousePosX, int mousePosY, int mouseLeftDown, int mouseLeftUp)
{
    // Change button state according to mouse position and button presses
    button->isActive = 0;
    if (button->rect.x <= mousePosX && mousePosX <= button->rect.x + button->rect.w &&
        button->rect.y <= mousePosY && mousePosY <= button->rect.y + button->rect.h)
    {
        button->isActive = 1;
        if(mouseLeftDown == 1)
            button->isPressed = 1;
    }
    if (mouseLeftUp == 1)
        button->isPressed = 0;
    
    // Change button color according to state
    if (button->isPressed == 1 && button -> brightness > 80)
        button -> brightness -= 0.2;
    else if (button->isActive == 1 && button->brightness > 100)
        button->brightness -= 0.1;
    else if (button->isActive == 1 && button->brightness < 100)
        button->brightness += 0.2;
    else if (button->isActive == 0 && button->brightness < BASE_BRIGHTNESS)
        button->brightness += 0.1;
}

int min(int x, int y)
{
    if (x > y)
        return y;
    return x;
}
