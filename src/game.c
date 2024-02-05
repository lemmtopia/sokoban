#include "defs.h"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <stdint.h>
#include <SDL2/SDL_image.h>

int last_frame_time;

// Player stuff
int last_right = 0;
int last_left = 0;
int last_down = 0;
int last_up = 0;

int hsp = 0;
int vsp = 0;

SDL_Surface* player_surf;
SDL_Surface* box_surf;
SDL_Surface* wall_surf;
    SDL_Surface* bg_surf;

SDL_Texture* player_tex;
SDL_Texture* box_tex;
SDL_Texture* wall_tex;
SDL_Texture* bg_tex;

struct {
    int x[MAX_TILES];
    int y[MAX_TILES];
    int type[MAX_TILES];

    int player_id;
    int num_tiles;
} game_data;

int check_aabb(int tile1, int tile2, int xx, int yy);
void player_movement();

void game_initialize(SDL_Renderer* renderer) {
    game_data.player_id = 0;
    game_data.num_tiles = 1;

    // Just to be sure on Windows
    for (int i = 0; i < game_data.num_tiles; i++) {
        game_data.x[i] = 0; 
        game_data.y[i] = 0; 
        game_data.type[i] = 0; 
    }

    game_data.num_tiles++;
    game_data.x[game_data.num_tiles - 1] = TILE_SIZE * SCALE * 2; 
    game_data.y[game_data.num_tiles - 1] = TILE_SIZE * SCALE; 
    game_data.type[game_data.num_tiles - 1] = TYPE_BOX;

    game_data.num_tiles++;
    game_data.x[game_data.num_tiles - 1] = TILE_SIZE * SCALE * 4; 
    game_data.y[game_data.num_tiles - 1] = TILE_SIZE * SCALE; 
    game_data.type[game_data.num_tiles - 1] = TYPE_BOX;

    game_data.num_tiles++;
    game_data.x[game_data.num_tiles - 1] = TILE_SIZE * SCALE * 4; 
    game_data.y[game_data.num_tiles - 1] = TILE_SIZE * SCALE * 3; 
    game_data.type[game_data.num_tiles - 1] = TYPE_UNDEFINED;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            game_data.num_tiles++;
            game_data.x[game_data.num_tiles - 1] = TILE_SIZE * SCALE * i; 
            game_data.y[game_data.num_tiles - 1] = TILE_SIZE * SCALE * j; 
            game_data.type[game_data.num_tiles - 1] = TYPE_BG;
        }
    }

    player_surf = IMG_Load("assets/player.png");
    wall_surf = IMG_Load("assets/wall.png");
    box_surf = IMG_Load("assets/box.png");
    bg_surf = IMG_Load("assets/bg.png");

    player_tex = SDL_CreateTextureFromSurface(renderer, player_surf);
    wall_tex = SDL_CreateTextureFromSurface(renderer, wall_surf);
    bg_tex = SDL_CreateTextureFromSurface(renderer, bg_surf);
    box_tex = SDL_CreateTextureFromSurface(renderer, box_surf);

    SDL_FreeSurface(player_surf);
    SDL_FreeSurface(box_surf);
    SDL_FreeSurface(bg_surf);
    SDL_FreeSurface(wall_surf);
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

    player_movement();
}

void game_draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < game_data.num_tiles; i++) {
        SDL_Rect tile = {game_data.x[i], game_data.y[i], TILE_SIZE * SCALE, TILE_SIZE * SCALE};
        if (game_data.type[i] == TYPE_BG) SDL_RenderCopy(renderer, bg_tex, NULL, &tile);
    }

    for (int i = 0; i < game_data.num_tiles; i++) {
        SDL_Rect tile = {game_data.x[i], game_data.y[i], TILE_SIZE * SCALE, TILE_SIZE * SCALE};

        if (i == game_data.player_id) {
            SDL_RenderCopy(renderer, player_tex, NULL, &tile);
        } else {
            if (game_data.type[i] == TYPE_BOX) SDL_RenderCopy(renderer, box_tex, NULL, &tile);
            else if (game_data.type[i] == TYPE_UNDEFINED) SDL_RenderCopy(renderer, wall_tex, NULL, &tile);
        }
    }

    SDL_RenderPresent(renderer);
}

int check_aabb(int tile1, int tile2, int xx, int yy) {
    int in_right = game_data.x[tile1] + TILE_SIZE * SCALE + xx > game_data.x[tile2]; 
    int in_left = game_data.x[tile1] + xx < game_data.x[tile2] + TILE_SIZE * SCALE; 
    int in_bottom = game_data.y[tile1] + TILE_SIZE * SCALE + yy > game_data.y[tile2]; 
    int in_top = game_data.y[tile1] + yy < game_data.y[tile2] + TILE_SIZE * SCALE; 

    return in_left && in_right && in_top && in_bottom;
}

void player_movement() {
    const uint8_t* state = SDL_GetKeyboardState(0);

    hsp = 0;
    vsp = 0;

    int box;
    int box2;

    if (state[SDL_SCANCODE_D] != last_right && state[SDL_SCANCODE_D]) {
        hsp = SCALE * TILE_SIZE;
        last_right = state[SDL_SCANCODE_D];
    } 

    if (state[SDL_SCANCODE_A] != last_left && state[SDL_SCANCODE_A]) {
        hsp = -SCALE * TILE_SIZE;
        last_left = state[SDL_SCANCODE_A];
    }
    if (state[SDL_SCANCODE_S] != last_down && state[SDL_SCANCODE_S]) {
        vsp = TILE_SIZE * SCALE;
        last_down = state[SDL_SCANCODE_S];
    } 

    if (state[SDL_SCANCODE_W] != last_up && state[SDL_SCANCODE_W]) {
        vsp = -TILE_SIZE * SCALE;
        last_up = state[SDL_SCANCODE_W];
    } 

    for (int i = game_data.player_id + 1; i < game_data.num_tiles; i++) {
        box = check_aabb(game_data.player_id, i, sign(hsp), sign(vsp)) && game_data.type[i] != TYPE_BG;

        for (int j = 0; j < game_data.num_tiles; j++) {
            if (j != i) {
                box2 = check_aabb(i, j, sign(hsp), sign(vsp)) && game_data.type[j] != TYPE_BG;
                if (box2) break;
            }
        }

        if (box) {
            if (!box2 && game_data.type[i] == TYPE_BOX) {
                game_data.x[i] += hsp;
                game_data.y[i] += vsp;
            }

            hsp = 0;
            vsp = 0;
            break;
        }
    }

    if (hsp != 0) vsp = 0;
    if (vsp != 0) hsp = 0;

    if (!state[SDL_SCANCODE_D]) last_right = 0;
    if (!state[SDL_SCANCODE_A]) last_left = 0;
    if (!state[SDL_SCANCODE_S]) last_down = 0;
    if (!state[SDL_SCANCODE_W]) last_up = 0;

    game_data.x[game_data.player_id] += hsp;
    game_data.y[game_data.player_id] += vsp;
}
