#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define PADDLE_WIDTH   20
#define PADDLE_HEIGHT  120
#define PADDLE_SPEED   6

#define BALL_SIZE      15
#define BALL_SPEED     4.0f   


void move_paddle(SDL_Rect *p, int dy) {
    p->y += dy;

    if (p->y < 0)
        p->y = 0;
    if (p->y + p->h > WINDOW_HEIGHT)
        p->y = WINDOW_HEIGHT - p->h;
}


void reset_ball(SDL_Rect *ball, float *vx, float *vy, int dir) {
    ball->x = (WINDOW_WIDTH - BALL_SIZE) / 2;
    ball->y = (WINDOW_HEIGHT - BALL_SIZE) / 2;

    *vx = dir * BALL_SPEED;
    *vy = 0.0f;
}

void move_ball(SDL_Rect *b, float *vx, float *vy,
               SDL_Rect *p1, SDL_Rect *p2)
{
    
    b->x += (int)(*vx);
    b->y += (int)(*vy);

    
    if (b->y <= 0) {
        b->y = 0;
        *vy = -*vy;
    }
    if (b->y + b->h >= WINDOW_HEIGHT) {
        b->y = WINDOW_HEIGHT - b->h;
        *vy = -*vy;
    }

    
    if (SDL_HasIntersection(b, p1)) {
        b->x = p1->x + p1->w;

        float paddle_center = p1->y + p1->h / 2.0f;
        float ball_center   = b->y + b->h / 2.0f;
        float offset        = (ball_center - paddle_center) / (p1->h / 2.0f);

        *vx = BALL_SPEED;
        *vy = offset * BALL_SPEED;
    }

    
    if (SDL_HasIntersection(b, p2)) {
        b->x = p2->x - b->w;

        float paddle_center = p2->y + p2->h / 2.0f;
        float ball_center   = b->y + b->h / 2.0f;
        float offset        = (ball_center - paddle_center) / (p2->h / 2.0f);

        *vx = -BALL_SPEED;
        *vy = offset * BALL_SPEED;
    }

    
    float length = sqrtf((*vx) * (*vx) + (*vy) * (*vy));
    if (length != 0.0f) {
        *vx = (*vx / length) * BALL_SPEED;
        *vy = (*vy / length) * BALL_SPEED;
    }

    
    if (b->x < 0)
        reset_ball(b, vx, vy, 1);
    if (b->x > WINDOW_WIDTH)
        reset_ball(b, vx, vy, -1);
}


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Ping Pong (Constant Speed)",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_Rect p1 = {
        40,
        (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2,
        PADDLE_WIDTH,
        PADDLE_HEIGHT
    };

    SDL_Rect p2 = {
        WINDOW_WIDTH - 40 - PADDLE_WIDTH,
        (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2,
        PADDLE_WIDTH,
        PADDLE_HEIGHT
    };

    SDL_Rect ball = {
        (WINDOW_WIDTH - BALL_SIZE) / 2,
        (WINDOW_HEIGHT - BALL_SIZE) / 2,
        BALL_SIZE,
        BALL_SIZE
    };

    float ball_vx = BALL_SPEED;
    float ball_vy = 0.0f;

    int running = 1;
    SDL_Event event;

    while (running) {
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;
        }
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_W])
            move_paddle(&p1, -PADDLE_SPEED);
        if (keys[SDL_SCANCODE_S])
            move_paddle(&p1,  PADDLE_SPEED);

        if (keys[SDL_SCANCODE_UP])
            move_paddle(&p2, -PADDLE_SPEED);
        if (keys[SDL_SCANCODE_DOWN])
            move_paddle(&p2,  PADDLE_SPEED);

        
        move_ball(&ball, &ball_vx, &ball_vy, &p1, &p2);

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &p1);
        SDL_RenderFillRect(renderer, &p2);
        SDL_RenderFillRect(renderer, &ball);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
