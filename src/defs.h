#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL.h>

#define TRUE 1
#define FALSE 0

#define MAX_TILES 32
#define TILE_SIZE 8

#define WIDTH 320
#define HEIGHT 180
#define SCALE 4

#define TYPE_UNDEFINED 0
#define TYPE_BOX 1

void game_initialize();
int game_process_input();
void game_update();
void game_draw(SDL_Renderer* renderer);

int sign(int n);

#endif
