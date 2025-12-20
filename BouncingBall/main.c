#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

#undef main   // REQUIRED on Windows + MinGW

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

    int running = 1;
    SDL_Event event;

    Uint32 last = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - last > 16) { 
            last = now;

            /
        }

        SDL_Delay(1);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
