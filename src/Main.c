#include "stdio.h"
#include "SDL2/SDL.h"

int main(int argc, char *argv[])
{
    int xres = 500, yres = 500;
    SDL_Window* win = SDL_CreateWindow("CMarch!", 100, 100, xres, yres, SDL_WINDOW_SHOWN);
    printf("Why didn't I begin this earlier?\n");
    SDL_DestroyWindow(win);
	SDL_Quit();
    return 0;
}
