#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <SDL/SDL.h>
#include <stdint.h>
#include "cl_game_object.h"
#include "cl_fonts.h"

#define TICK_INTERVAL 30

// Game pages
#define STARTUP_SCREEN_PAGE 0
#define GAME_PAGE 1

// Startup menu items
#define LOGIN_MENU_ITEM 0
#define PASSWORD_MENU_ITEM 1
#define START_MENU_ITEM 2

struct game_state
{
  int game_running;

  int page;

  struct player *player;

  // Other players on scene
  uint16_t other_players_count;
  struct player *other_players;

  // To control frame rate
  Uint32 next_time;

  // All graphic game objects to draw on scene
  uint16_t game_objects_count;
  struct game_object **scene_game_objects;

  // Game events object
  SDL_Event event;

  // Background and screen for rendering
  SDL_Surface *screen;
  SDL_Surface *background;

  // Startup menu
  SDL_Surface *startup_menu;
  SDL_Surface *underscore;

  int current_menu;
};

void add_game_object_on_scene(struct game_object *game_object, struct game_state **game_state_ptr_ptr);
void cl_print_game_objects_on_scene(struct game_state *game_state_ptr);
Uint32 time_left(Uint32 next_time);

void cl_startup_menu_init(struct game_state *game_state);
void cl_startup_menu_draw(struct game_state *game_state, struct player *player);
void cl_startup_menu_keyboard_handler(struct game_state *game_state, struct player *player, int x, int y, int dimension, char *key);

#endif