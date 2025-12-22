#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define WIDTH  1000
#define HEIGHT 800

#define COLOR_BLUE (SDL_Color){0, 0, 255, 255}

typedef struct Circle {
    double x;
    double y;
    double radius;
} Circle;

void FillCircle(SDL_Surface *surface, Circle circle, SDL_Color color) {
    Uint32 pixelColor = SDL_MapRGB(
        surface->format, color.r, color.g, color.b
    );

    SDL_LockSurface(surface);

    Uint32 *pixels = (Uint32 *)surface->pixels;
    int pitch = surface->pitch / 4; 
    for (int y = 0; y < surface->h; y++) {
        for (int x = 0; x < surface->w; x++) {

            double dx = x - circle.x;
            double dy = y - circle.y;

            double distance = sqrt(dx * dx + dy * dy);

            if (distance <= circle.radius) {
                pixels[y * pitch + x] = pixelColor;
            }
        }
    }

    SDL_UnlockSurface(surface);
}

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Circle using math.h",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    if (!surface) {
        printf("Surface Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

   
    SDL_FillRect(
        surface,
        NULL,
        SDL_MapRGB(surface->format, 0, 0, 0)
    );

   
    Circle circle = {200, 200, 80};

    
    FillCircle(surface, circle, COLOR_BLUE);
    SDL_UpdateWindowSurface(window);

    
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
