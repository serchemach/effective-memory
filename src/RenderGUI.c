#include "RenderGUI.h"
#include "stdio.h"
#include "SDL2/SDL.h"
#include "Button.h"

void startRender()
{
    int xres = 500, yres = 500;
    SDL_Window* win; SDL_Renderer* renderer;
    InitialiseRender(xres, yres, &win, &renderer);

    Button testButton = CreateButton(10, 10, 100, 100, NULL);

    SDL_Event event;
	int running = 1;
    int mousePosX = 0, mousePosY = 0;
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&event))
    	{
            printf("%d ", event.type);
            switch (event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEMOTION:
                    mousePosX = event.motion.x;
                    mousePosY = event.motion.y;
                    printf("%d, %d, %d\n", mousePosX, mousePosY, testButton.isActive);
                    break;
            }
        }

        UpdateButton(&testButton, mousePosX, mousePosY);
        RenderButton(renderer, &testButton);

        SDL_RenderPresent(renderer);
    }   

    ExitRender(win);
}

void InitialiseRender(int xres, int yres, SDL_Window** rWin, SDL_Renderer** rRenderer)
{
    SDL_Window* win = SDL_CreateWindow("QuCalc", 100, 100, xres, yres, SDL_WINDOW_HIDDEN);

    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
