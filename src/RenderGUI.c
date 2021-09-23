#include "stdio.h"
#include "string.h"

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL_TTF/SDL_ttf.h"

#include "RenderGUI.h"
#include "RenderGL.h"
#include "Button.h"
#include "TextBox.h"
#include "Utils.h"
 
void startRender()
{
    int xres = 500, yres = 500;
    SDL_Window* win; SDL_Renderer* renderer; SDL_Surface* windowSurface; SDL_Surface* renderSurface;
	SDL_GLContext glcontext = (SDL_GLContext) {0};
    InitialiseRender(xres, yres, &win, &renderer, &renderSurface, &windowSurface, &glcontext);
	
	startGLRender(renderSurface);

    int numberOfButtons = 10;
    Button guiButtons[numberOfButtons];
    // Create digits
    for (int i = 1; i < 10; ++i)
    {
        char tempStr[2];
        tempStr[0] = 48 + i;
        tempStr[1] = '\0';
        guiButtons[i] = CreateButton(renderer, (int)((float) ((i - 1) % 3 + 3) / 16 + (float)((i - 1) % 3) * xres * 11 / 64), 
        500 - (int)((float)((i - 1) / 3 + 2) / 16 + (float)((i - 1) / 3 + 2) * yres / 7),
        (float)xres * 11 / 64, (float)yres / 7, tempStr);
    }
    guiButtons[0] = CreateButton(renderer, (int)((float) 4 / 16 + (float)xres * 11 / 64), 
        500 - (int)((float)2 / 16 + (float)yres / 7),
        (float)xres * 11 / 64, (float)yres / 7, "0");
    


    // Button testButton = CreateButton(renderer, 10, 10, 100, 100, "1");
    TextBox testTBox = CreateTextBox(renderer, 20, 20, 100, 60, "134567");

    SDL_Event event;
	int running = 1;
    int mousePosX = 0, mousePosY = 0, mouseLeftDown = 0, mouseLeftUp = 0, lastKeyCode = 0, backspacePressed = 0;
    printf("123\n");
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, BG_BRIGHTNESS, BG_BRIGHTNESS, BG_BRIGHTNESS, 255);
        SDL_RenderClear(renderer);
        mouseLeftDown = 0;
        mouseLeftUp = 0;
        lastKeyCode = 0;
        backspacePressed = 0;

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
                case SDL_TEXTINPUT:
                    lastKeyCode = event.text.text[0];
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_BACKSPACE)
                        backspacePressed = 1;
                    break;
            }
        }
        
        // UpdateButton(&testButton, mousePosX, mousePosY, mouseLeftDown, mouseLeftUp);
        // RenderButton(renderer, &testButton);

        for (int i = 0; i < numberOfButtons; ++i)
        {
            UpdateButton(&guiButtons[i], mousePosX, mousePosY, mouseLeftDown, mouseLeftUp);
            RenderButton(renderer, &guiButtons[i]);
        }

        UpdateTextBox(renderer, &testTBox, mousePosX, mousePosY, mouseLeftDown, lastKeyCode, backspacePressed);
        RenderTextBox(renderer, testTBox);

        SDL_RenderPresent(renderer);
        //SDL_BlitSurface(renderSurface, NULL, windowSurface, NULL);
		
        SDL_LockSurface(renderSurface);
		renderGL(xres, yres, renderSurface, 0, 0);
        SDL_UnlockSurface(renderSurface);
		
        //SDL_UpdateWindowSurface(win);
		SDL_GL_SwapWindow(win);
    }   

	destroyGLRender();
    ExitRender(win, glcontext);
}

void InitialiseRender(int xres, int yres, 
	SDL_Window** rWin, SDL_Renderer** rRenderer, SDL_Surface** rRenderSurface, SDL_Surface** rWindowSurface, 
	SDL_GLContext* rGlcontext)
{
    // I'm sorry for this function
    if(TTF_Init() != 0)
    {  
        printf("TTF not initialised\n");
        exit(1);
    }
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);
	
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

    SDL_Window* win = SDL_CreateWindow("QuCalc", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, xres, yres, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
	
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);
	SDL_GL_MakeCurrent(win, glcontext);
	if(glewInit() != GLEW_OK) {
		printf("GLEW not initialised\n");
        exit(1);
	}

    SDL_Surface* windowSurface = SDL_GetWindowSurface(win);
    SDL_Surface* renderSurface = SDL_CreateRGBSurfaceWithFormat(0, xres, yres, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Renderer* renderer = SDL_CreateSoftwareRenderer(renderSurface);
    
    SDL_SetRenderDrawColor(renderer, BG_BRIGHTNESS, BG_BRIGHTNESS, BG_BRIGHTNESS, 255);
    SDL_RenderClear(renderer);

    SDL_ShowWindow(win);
    SDL_BlitSurface(renderSurface, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(win);
	
    *rWin = win; *rRenderer = renderer;
    *rRenderSurface = renderSurface; *rWindowSurface = windowSurface;
	*rGlcontext = glcontext;
}
 
void ExitRender(SDL_Window* win, SDL_GLContext glcontext)
{
	SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(win);
	SDL_Quit();
}
