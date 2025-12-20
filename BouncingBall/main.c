#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

#undef main

void create

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Bounce",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);

    SDL_Rect rect = {300, 300, 20, 20};
    int vx = 4;
    int vy = 4;

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        
        rect.x += vx;
        rect.y += vy;

        
        if (rect.x <= 0 || rect.x + rect.w >= surface->w)
            vx = -vx;
        if (rect.y <= 0 || rect.y + rect.h >= surface->h)
            vy = -vy;

        
        SDL_FillRect(surface, NULL, 0x000000);

        
        SDL_FillRect(surface, &rect, 0xFFFFFFFF);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16); 
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
