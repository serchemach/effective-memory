#include "stdio.h"
#include "TextBox.h"
#include "SDL_TTF/SDL_ttf.h"
#include "string.h"
#include "stdlib.h"
#include "Utils.h"
#include "math.h"

TextBox CreateTextBox(SDL_Renderer* renderer, int xPos, int yPos, int width, int height, char* text)
{
    SDL_Rect rect = {xPos, yPos, width, height};
    TextBox result;
    result.rect = rect;

    result.brightness = TBOX_BASE_BRIGHTNESS;
    result.isActive = 0;
    result.isUsed = 0;
    result.hasADot = 0;

    result.text = malloc(sizeof(char) * TBOX_TEXT_LENGTH);
    result.text = strcpy(result.text, text);
    result.curTextSize = strlen(result.text);

    result.textFRect.y = 0; result.textFRect.x = 0;
    result.textTexture = NULL;
    UpdateTextBoxTextTexture(renderer, &result);
    return result;
}


void RenderTextBox(SDL_Renderer* renderer, TextBox tBox)
{
    // Draw the main body of tBox
    SDL_SetRenderDrawColor(renderer, roundf(tBox.brightness), roundf(tBox.brightness), roundf(tBox.brightness), 255);
    SDL_RenderFillRect(renderer, &tBox.rect);
    
    // Draw the outline
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderDrawRect(renderer, &tBox.rect);

    // Render the text texture
    SDL_RenderCopy(renderer, tBox.textTexture, &tBox.textFRect, &tBox.textDRect);
}

void UpdateTextBox(SDL_Renderer* renderer, TextBox* tBox, int mousePosX, int mousePosY, int mouseLeftDown, char lastChar, int backspacePressed)
{
    // Determine the state
    tBox->isActive = 0;

    if (tBox->rect.x + tBox->rect.w >= mousePosX && tBox->rect.x <= mousePosX &&
        tBox->rect.y + tBox->rect.h >= mousePosY && tBox->rect.y <= mousePosY)
    {
        tBox->isActive = 1;
        if (mouseLeftDown == 1)
            tBox->isUsed = 1;
    }
    
    // Adjust brightness for the current frame
    if (tBox->isUsed == 1)
    {
        if (tBox->brightness > 50)
            tBox->brightness -= 1;
    }
    else 
    {   
        if (tBox->isActive == 1 && tBox->brightness > 60)
            tBox->brightness -= 0.6;
        else if (tBox->isActive == 1 && 60 - tBox->brightness > 1)
            tBox->brightness += 1;
        else if (tBox->isActive == 0 && tBox->brightness < TBOX_BASE_BRIGHTNESS)
            tBox->brightness += 0.6;
    }

    // Do the keys polling and text texture shift adjustment    
    int tWidth, tHeight, pressedDigit;
    switch(tBox->isUsed)
    {
        case 1:
            // Adjust texture shift
            SDL_QueryTexture(tBox->textTexture, NULL, NULL, &tWidth, &tHeight);
            if (tBox->textFRect.x < tWidth - tBox->textFRect.w)
                tBox->textFRect.x += 1;
            else if (tBox->textFRect.x > tWidth - tBox->textFRect.w + 1)
                tBox->textFRect.x -= 1;

            // Poll the keys
            if (backspacePressed == 1 && tBox->curTextSize > 0 && *(tBox->text + tBox->curTextSize) != '-')
            {
                tBox->curTextSize--;
                if (*(tBox->text + tBox->curTextSize) == '.')
                    tBox->hasADot = 0;

                if (tBox->curTextSize == 0)
                {
                    *(tBox->text + tBox->curTextSize) = '0';
                    tBox->curTextSize = 1;
                }
                else
                    *(tBox->text + tBox->curTextSize) = '\0';

                // Update the text texture
                UpdateTextBoxTextTexture(renderer, tBox);
            }
            else if (lastChar == '-' && tBox->text[0] != '-')
            {
                for(int i = tBox->curTextSize; i > 0; i--)
                {
                    tBox->text[i] = tBox->text[i - 1];
                }
                tBox->curTextSize++;
                tBox->text[0] = '-';
                tBox->text[tBox->curTextSize] = '\0';
                UpdateTextBoxTextTexture(renderer, tBox);
            }
            else if (lastChar == '+' && tBox->text[0] == '-')
            {
                for(int i = 0; i < tBox->curTextSize; i++)
                {
                    tBox->text[i] = tBox->text[i + 1];
                }
                tBox->curTextSize--;
                UpdateTextBoxTextTexture(renderer, tBox);
            }
            else if (((lastChar <= 57 && lastChar >= 48) || (lastChar == '.' && tBox->hasADot == 0)) && tBox->curTextSize < TBOX_TEXT_LENGTH)
            {
                if(lastChar == '.')
                    tBox->hasADot = 1;

                if (tBox->curTextSize == 1 && tBox->text[0] == '0')
                    tBox->text[0] = lastChar;
                else
                {
                    *(tBox->text + tBox->curTextSize) = lastChar;
                    tBox->curTextSize++;
                    *(tBox->text + tBox->curTextSize) = '\0';
                }
                // Update the text texture
                UpdateTextBoxTextTexture(renderer, tBox);
            }

            break;

        case 0:
            if (tBox->textFRect.x > 0)
                tBox->textFRect.x -= 1;
            break;
    }

}

void UpdateTextBoxText(SDL_Renderer* renderer, TextBox* tBox, char* newText)
{
    tBox->text = strcpy(tBox->text, newText);
    tBox->curTextSize = strlen(tBox->text);
    UpdateTextBoxTextTexture(renderer, tBox);
}

void UpdateTextBoxTextTexture(SDL_Renderer* renderer, TextBox* tBox)
{
    if (tBox->textTexture)
        SDL_DestroyTexture(tBox->textTexture);
    
    tBox->textTexture = CreateTextTexture(renderer, tBox->rect.h, tBox->text);
    int tWidth, tHeight;
    SDL_QueryTexture(tBox->textTexture, NULL, NULL, &tWidth, &tHeight);

    tBox->textDRect.h = tBox->rect.h;
    tBox->textDRect.w = (int)((float)tWidth * tBox->rect.h / tHeight);
    tBox->textDRect.x = tBox->rect.x;
    tBox->textDRect.y = tBox->rect.y;

    tBox->textFRect.h = tHeight;
    tBox->textFRect.w = tWidth;

    if (tBox->textDRect.w > tBox->rect.w)
    {
        tBox->textFRect.w = (int)((float)tBox->rect.w * tHeight / tBox->rect.h);       
        tBox->textDRect.w = tBox->rect.w; 
    }
}
