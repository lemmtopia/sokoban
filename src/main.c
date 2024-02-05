#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <stdio.h>

#include "defs.h"
#include <SDL2/SDL_image.h>

SDL_Window* window;
SDL_Renderer* renderer;
int is_running;

int initialize_sdl();

int main(int argc, char* argv[]) {
    is_running = initialize_sdl();
    
    game_initialize(renderer);
    while (is_running) {
        is_running = game_process_input();
        game_update();
        game_draw(renderer);
    }

    return 0;
}

int initialize_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Could not intitalize SDL\n");
        return FALSE;
    }

    window = SDL_CreateWindow("Sokoban",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH * SCALE,
            HEIGHT * SCALE,
            0);

    if (!window) {
        printf("Could not create the window\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        printf("Could not create the window\n");
        return FALSE;
    }

    IMG_Init(IMG_INIT_PNG);

    return TRUE;
}
