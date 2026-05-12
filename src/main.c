#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define ASTEROID_COUNT 6

#define PI 3.14159265f

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

typedef struct {
    float x;
    float y;
} Vector2D;

typedef struct {
    Vector2D pos;
    Vector2D vel;
    float angle;
    float size;
} Spaceship;

typedef struct {
    Vector2D pos;
    Vector2D vel;
    float radius;
} Asteroid;

void WrapPosition(Vector2D* pos) {
    if (pos->x < 0) pos->x = SCREEN_WIDTH;
    if (pos->x > SCREEN_WIDTH) pos->x = 0;
    if (pos->y < 0) pos->y = SCREEN_HEIGHT;
    if (pos->y > SCREEN_HEIGHT) pos->y = 0;
}

void DrawSpaceship(SDL_Renderer* renderer, Spaceship ship) {
    float x1 = ship.pos.x + cos(ship.angle) * ship.size;
    float y1 = ship.pos.y + sin(ship.angle) * ship.size;

    float x2 = ship.pos.x + cos(ship.angle + 2.5f) * ship.size;
    float y2 = ship.pos.y + sin(ship.angle + 2.5f) * ship.size;

    float x3 = ship.pos.x + cos(ship.angle - 2.5f) * ship.size;
    float y3 = ship.pos.y + sin(ship.angle - 2.5f) * ship.size;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
    SDL_RenderDrawLine(renderer, (int)x2, (int)y2, (int)x3, (int)y3);
    SDL_RenderDrawLine(renderer, (int)x3, (int)y3, (int)x1, (int)y1);
}

void DrawAsteroid(SDL_Renderer* renderer, Asteroid asteroid) {
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

    for (int i = 0; i < 360; i += 20) {
        float angle1 = i * PI / 180.0f;
        float angle2 = (i + 20) * PI / 180.0f;

        int x1 = (int)(asteroid.pos.x + cos(angle1) * asteroid.radius);
        int y1 = (int)(asteroid.pos.y + sin(angle1) * asteroid.radius);

        int x2 = (int)(asteroid.pos.x + cos(angle2) * asteroid.radius);
        int y2 = (int)(asteroid.pos.y + sin(angle2) * asteroid.radius);

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }
}

int main(int argc, char* args[]) {
    SDL_SetMainReady();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL baslatilamadi! Hata: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Asteroids - KOU Proje",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        printf("Pencere olusturulamadi! Hata: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL) {
        printf("Renderer olusturulamadi! Hata: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Spaceship player;
    player.pos.x = SCREEN_WIDTH / 2.0f;
    player.pos.y = SCREEN_HEIGHT / 2.0f;
    player.vel.x = 0.0f;
    player.vel.y = 0.0f;
    player.angle = -PI / 2.0f;
    player.size = 20.0f;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT]) {
            player.angle -= 0.08f;
        }

        if (keys[SDL_SCANCODE_RIGHT]) {
            player.angle += 0.08f;
        }

        if (keys[SDL_SCANCODE_UP]) {
            player.vel.x += cos(player.angle) * 0.15f;
            player.vel.y += sin(player.angle) * 0.15f;
        }

        player.pos.x += player.vel.x;
        player.pos.y += player.vel.y;

        player.vel.x *= 0.99f;
        player.vel.y *= 0.99f;

        WrapPosition(&player.pos);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        DrawSpaceship(renderer, player);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}