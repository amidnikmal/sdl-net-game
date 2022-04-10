#ifndef __EVENTS__
#define __EVENTS__
#include <SDL/SDL.h>

#define LEFT -2
#define RIGHT 2
#define UP 1
#define DOWN -1
#define STEP 3

void event_handle(
    SDL_Event *event,
    void (*const clean_game)(void),
    void (*const on_change)(int x, int y, int dimension, char *key));

void keys_handler(
    SDL_Event event,
    void (*const on_change)(int x, int y, int dimension, char *key));

#endif
