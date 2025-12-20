#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>

#undef main

struct Circle {
    int x;
    int y;
    int radius;
};

void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 color) {
    int x0 = circle.x;
    int y0 = circle.y;
    int r  = circle.radius;

    Uint32 *pixels = (Uint32 *)surface->pixels;

    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                int px = x0 + x;
                int py = y0 + y;

                if (px >= 0 && px < surface->w &&
                    py >= 0 && py < surface->h) {
                    pixels[py * surface->w + px] = color;
                }
            }
        }
    }
}

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Bouncing Ball",
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

    struct Circle ball = {400, 300, 10};
    int vx = 4;
    int vy = 4;

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        
        ball.x += vx;
        ball.y += vy;

        
        if (ball.x - ball.radius <= 0 || ball.x + ball.radius >= surface->w)
            vx = -vx;
        if (ball.y - ball.radius <= 0 || ball.y + ball.radius >= surface->h)
            vy = -vy;

        
        SDL_FillRect(surface, NULL, 0x000000);

        
        SDL_LockSurface(surface);
        FillCircle(surface, ball, 0xFFFFFFFF);
        SDL_UnlockSurface(surface);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(16); 
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
