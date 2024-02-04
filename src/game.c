#include "defs.h"
#include <stdint.h>

int last_frame_time;

struct {
    int x[MAX_TILES];
    int y[MAX_TILES];
    int type[MAX_TILES];

    int player_id;
    int num_tiles;
} game_data;


void game_initialize() {
    game_data.player_id = 0;
    game_data.num_tiles = 1;

    // Just to be sure on Windows
    for (int i = 0; i < game_data.num_tiles; i++) {
        game_data.x[i] = 0; 
        game_data.y[i] = 0; 
        game_data.type[i] = 0; 
    }
}

int game_process_input() {
    SDL_Event ev;
    SDL_PollEvent(&ev);
    if (ev.type == SDL_QUIT) return FALSE;

    return TRUE;
}

void game_update() {
    float dt = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();

    const uint8_t* state = SDL_GetKeyboardState(0);
    game_data.x[game_data.player_id] += 8 * (state[SDL_SCANCODE_D] - state[SDL_SCANCODE_A]);
}

void game_draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //TODO: Draw stuff...

    for (int i = 0; i < game_data.num_tiles; i++) {
        SDL_Rect tile = {game_data.x[i], game_data.y[i], TILE_SIZE * SCALE, TILE_SIZE * SCALE};
        SDL_RenderFillRect(renderer, &tile);
    }

    SDL_RenderPresent(renderer);
}
