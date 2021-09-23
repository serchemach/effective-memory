#include "Button.h"
#include "SDL_TTF/SDL_ttf.h"
#include "stdio.h"
#include "Utils.h"
#include "stdlib.h"
#include "string.h"

Button CreateButton(SDL_Renderer* renderer, int xPos, int yPos, int width, int height, char* text)
{
    Button result;
    result.rect.x = xPos; result.rect.y = yPos;
    result.rect.w = width; result.rect.h = height;

    result.isActive = 0;
    result.isPressed = 0;

    result.text = malloc(sizeof(char) * 10);
    result.text = strcpy(result.text, text);
    result.brightness = BASE_BRIGHTNESS;
    
    result.textTexture = CreateTextTexture(renderer, min(height, width), text);
    int tWidth, tHeight;
    SDL_QueryTexture(result.textTexture, NULL, NULL, &tWidth, &tHeight);

    if ((float)tWidth / width > (float)tHeight / height)
    {
        result.textRect.w = width;
        result.textRect.h = (int)((float)(tHeight * width) / tWidth);
        result.textRect.y = yPos + (height - result.textRect.h) / 2;
        result.textRect.x = xPos + (width - result.textRect.w) / 2;
    }
    else
    {
        result.textRect.h = height;
        result.textRect.w = (int)((float)(tWidth * height) / tHeight);
        result.textRect.x = xPos + (width - result.textRect.w) / 2;
        result.textRect.y = yPos + (height - result.textRect.h) / 2;
    }

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
    button->isTriggering = 0;
    if (button->rect.x <= mousePosX && mousePosX <= button->rect.x + button->rect.w &&
        button->rect.y <= mousePosY && mousePosY <= button->rect.y + button->rect.h)
    {
        button->isActive = 1;
        if(mouseLeftDown == 1)
            button->isPressed = 1;
    }
    if (button->isPressed == 1 && mouseLeftUp == 1)
    {
        button->isPressed = 0;
        button->isTriggering = 1;
    }
    // Change button color according to state
    if (button->isPressed == 1 && button -> brightness > 80)
        button->brightness -= 1.5;
    else if (button->isActive == 1 && button->brightness > 100)
        button->brightness -= 1;
    else if (button->isActive == 1 && button->brightness < 100)
        button->brightness += 1.5;
    else if (button->isActive == 0 && button->brightness < BASE_BRIGHTNESS)
        button->brightness += 1;
}
