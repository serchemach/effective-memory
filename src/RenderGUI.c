#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL_TTF/SDL_ttf.h"

#include "RenderGUI.h"
#include "RenderGL.h"
#include "Button.h"
#include "TextBox.h"
#include "Utils.h"
#include "quaternion.h"
 
void startRender()
{
    int xres = 500, yres = 500;
    SDL_Window* win; SDL_Renderer* renderer; SDL_Surface* windowSurface; SDL_Surface* renderSurface;
	SDL_GLContext glcontext = (SDL_GLContext) {0};
    InitialiseRender(xres, yres, &win, &renderer, &renderSurface, &windowSurface, &glcontext);
	
	startGLRender(renderSurface);
	loadPreviewModel("mdl.obj");
	loadPreviewTexture("test.gif");

    struct Quaternion resultQuaternion = Quaternion_new(0, 0, 0, 0);
    struct Quaternion additonalQuaternion = Quaternion_new(0, 0, 0, 0);

    // Create textBoxes
    TextBox resultQTextBoxes[4];
    TextBox additionalQTextBoxes[4];
    for (int i = 0; i < 4; ++i)
    {
        resultQTextBoxes[i] = CreateTextBox(renderer, (int)((float)(i + 1)*xres / 16 + (float)i*xres/7),
        (int)((float)yres / 20), (int)((float)xres/5), (int)((float)yres / 10), "0");

        additionalQTextBoxes[i] = CreateTextBox(renderer, (int)((float)(i + 1)*xres / 16 + (float)i*xres/7),
        (int)((float)yres / 5), (int)((float)xres/5), (int)((float)yres / 10), "0");
    }
    TextBox* activeTextBox = &resultQTextBoxes[0];
    resultQTextBoxes[0].isUsed = 1;

    Button* activeButton = NULL;
    int numberOfButtons = 16;
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
    
    // Create operators
    char* tString = "+-*^";
    for (int i = 10; i < 14; ++i)
    {
        char tempStr[2];
        tempStr[0] = tString[i - 10];
        tempStr[1] = '\0';
        guiButtons[i] = CreateButton(renderer, (int)((float)3 / 8 + (float)xres * 33 / 64), 
            500 - (int)((float)((i - 10) + 2) / 16 + (float)(i - 9) * yres / 7), 
            (float)xres * 11 / 64, (float)yres / 7, tempStr);
    }
    guiButtons[14] = CreateButton(renderer, (int)((float) 6 / 16 + (float)xres * 11 / 32), 
            500 - (int)((float)2 / 16 + (float)yres / 7),
            (float)xres * 11 / 64, (float)yres / 7, ".");
    guiButtons[15] = CreateButton(renderer, (int)((float) 6 / 16), 
            500 - (int)((float)2 / 16 + (float)yres / 7),
            (float)xres * 11 / 64, (float)yres / 7, "+-");


    // Button testButton = CreateButton(renderer, 10, 10, 100, 100, "1");
    //TextBox testTBox = CreateTextBox(renderer, 20, 20, 100, 60, "134567");

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

        // Update the buttons and determine the active one
        for (int i = 0; i < numberOfButtons; ++i)
        {
            UpdateButton(&guiButtons[i], mousePosX, mousePosY, mouseLeftDown, mouseLeftUp);

            if (guiButtons[i].isTriggering == 1)
                activeButton = &guiButtons[i];
            RenderButton(renderer, &guiButtons[i]);
        }
        if (activeButton != NULL && activeButton->isTriggering == 0)
            activeButton = NULL;
        
        // Poll buttons
        if(activeButton != NULL)
        {
            printf("%s\n", activeButton->text);
            if (activeButton->text == "+-")
            {
                if (activeTextBox->text[0] == '-')
                    lastKeyCode = '+';
                else
                    lastKeyCode = '-';
            }
            else if ((activeButton->text[0] <= 57 && activeButton->text[0] >= 48) || 
                    activeButton->text[0] == '.')
                lastKeyCode = activeButton->text[0];
            else
            {
                switch(activeButton->text[0])
                {
                    case '+':
                        resultQuaternion = Quaternion_addQuaternion(resultQuaternion, additonalQuaternion);
                        break;
                    case '-':
                        resultQuaternion = Quaternion_subQuaternion(resultQuaternion, additonalQuaternion);
                        break;
                    case '*':
                        resultQuaternion = Quaternion_mulQuaternion(resultQuaternion, additonalQuaternion);
                        break;
                    case '^':
                        resultQuaternion = Quaternion_exp(resultQuaternion);
                        break;
                }
                if (activeButton->text[0] == '+' || activeButton->text[0] == '-' || activeButton->text[0] == '*' || activeButton->text[0] == '^')
                {
                    char tmp[50];
                    sprintf(tmp, "%g", resultQuaternion.x);
                    UpdateTextBoxText(renderer, &resultQTextBoxes[0], tmp);

                    sprintf(tmp, "%g", resultQuaternion.y);
                    UpdateTextBoxText(renderer, &resultQTextBoxes[1], tmp);

                    sprintf(tmp, "%g", resultQuaternion.z);
                    UpdateTextBoxText(renderer, &resultQTextBoxes[2], tmp);

                    sprintf(tmp, "%g", resultQuaternion.w);
                    UpdateTextBoxText(renderer, &resultQTextBoxes[3], tmp);
                }
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            UpdateTextBox(renderer, &resultQTextBoxes[i], mousePosX, mousePosY, mouseLeftDown, lastKeyCode, backspacePressed);
            if (resultQTextBoxes[i].isUsed == 1 && activeTextBox != &resultQTextBoxes[i])
            {
                activeTextBox->isUsed = 0;
                activeTextBox = &resultQTextBoxes[i];
            }
            if (activeButton != NULL && (activeButton->text[0] == '+' || activeButton->text[0] == '-' || activeButton->text[0] == '*' || activeButton->text[0] == '^'))
                resultQTextBoxes[i].brightness += 3;
            RenderTextBox(renderer, resultQTextBoxes[i]);

            UpdateTextBox(renderer, &additionalQTextBoxes[i], mousePosX, mousePosY, mouseLeftDown, lastKeyCode, backspacePressed);
            if (additionalQTextBoxes[i].isUsed == 1 && activeTextBox != &additionalQTextBoxes[i])
            {
                activeTextBox->isUsed = 0;
                activeTextBox = &additionalQTextBoxes[i];
            }
            RenderTextBox(renderer, additionalQTextBoxes[i]);
        }

        // UpdateTextBox(renderer, &testTBox, mousePosX, mousePosY, mouseLeftDown, lastKeyCode, backspacePressed);
        // RenderTextBox(renderer, testTBox);

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
	
	IMG_Init(0);
	
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
	IMG_Quit();
    SDL_DestroyWindow(win);
	SDL_Quit();
}
