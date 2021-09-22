#include "Button.h"

Button CreateButton(int xPos, int yPos, int width, int height, char* text)
{
    Button result;
    result.rect.h = height; result.rect.w = width;
    result.rect.x = xPos; result.rect.y = yPos;
    
    result.isActive = 0;
    result.text = text;
    result.brightness = BASE_BRIGHTNESS;
    
    return result;
}

void RenderButton(SDL_Renderer* renderer, Button* button)
{
    SDL_SetRenderDrawColor(renderer, (int)button->brightness, (int)button->brightness, (int)button->brightness, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderDrawRect(renderer, &button->rect);
}

void UpdateButton(Button* button, int mousePosX, int mousePosY)
{
    button->isActive = 0;
    if (button->rect.x <= mousePosX && mousePosX <= button->rect.x + button->rect.w &&
        button->rect.y <= mousePosY && mousePosY <= button->rect.y + button->rect.h)
    {
        button->isActive = 1;
    }
    
    if (button->isActive == 1 && button->brightness > 100)
    {
        button->brightness -= 0.1;
    }
    else if (button->isActive == 0 && button->brightness < BASE_BRIGHTNESS)
    {
        button->brightness += 0.1;
    }
}
