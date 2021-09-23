#include "RenderGUI.h"
#include "stdio.h"
#include "SDL2/SDL.h"
#include "SDL_TTF/SDL_ttf.h"
#include "Button.h"
#include "TextBox.h"
#include "Utils.h"

void startRender()
{
    int xres = 500, yres = 500;
    SDL_Window* win; SDL_Renderer* renderer;
    InitialiseRender(xres, yres, &win, &renderer);

    Button testButton = CreateButton(renderer, 10, 10, 100, 100, "1");
    TextBox testTBox = CreateTextBox(renderer, 200, 200, 100, 60, "tBo134567");

    SDL_Event event;
	int running = 1;
    int mousePosX = 0, mousePosY = 0, mouseLeftDown = 0, mouseLeftUp = 0, lastKeyCode = 0;
    printf("123\n");
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, BG_BRIGHTNESS, BG_BRIGHTNESS, BG_BRIGHTNESS, 255);
        SDL_RenderClear(renderer);
        mouseLeftDown = 0;
        mouseLeftUp = 0;
        lastKeyCode = 0;

        while (SDL_PollEvent(&event))
    	{
            switch (event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEMOTION:
                    mousePosX = event.motion.x;
                    mousePosY = event.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        mouseLeftDown = 1;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        mouseLeftUp = 1;
                    break;
                case SDL_KEYDOWN:
                    lastKeyCode = event.key.keysym.scancode;
                    break;
            }
        }
        
        UpdateButton(&testButton, mousePosX, mousePosY, mouseLeftDown, mouseLeftUp);
        UpdateTextBox(renderer, &testTBox, mousePosX, mousePosY, mouseLeftDown, lastKeyCode);

        RenderTextBox(renderer, testTBox);
        RenderButton(renderer, &testButton);

        SDL_RenderPresent(renderer);
    }   

    ExitRender(win);
}

void InitialiseRender(int xres, int yres, SDL_Window** rWin, SDL_Renderer** rRenderer)
{
    // I'm sorry for this function
    if(TTF_Init() != 0)
    {  
        printf("TTF not initialised\n");
        exit(1);
    }

    SDL_Window* win = SDL_CreateWindow("QuCalc", 100, 100, xres, yres, SDL_WINDOW_HIDDEN);

    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_SetRenderDrawColor(renderer, BG_BRIGHTNESS, BG_BRIGHTNESS, BG_BRIGHTNESS, 255);
    SDL_RenderClear(renderer);

    SDL_ShowWindow(win);
    SDL_RenderPresent(renderer);
    *rWin = win; *rRenderer = renderer;


}
 
void ExitRender(SDL_Window* win)
{
    SDL_DestroyWindow(win);
	SDL_Quit();
}
