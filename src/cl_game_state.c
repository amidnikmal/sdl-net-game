#include "cl_game_state.h"
#include "cl_player.h"

// Double pointer needed because `add_game_object_on_scene` called from nested func
void add_game_object_on_scene(struct game_object *game_object, struct game_state **game_state_ptr_ptr)
{
  struct game_state *game_state_ptr = *game_state_ptr_ptr;
  uint16_t *count = &(game_state_ptr->game_objects_count);

  if (*count > 0)
  {
    game_state_ptr->scene_game_objects = (struct game_object **)realloc(game_state_ptr->scene_game_objects, ((*count) + 1) * sizeof(struct game_object *));
  }
  else
  {
    game_state_ptr->scene_game_objects = (struct game_object **)malloc(sizeof(struct game_object **));
  }

  struct game_object **ptr = game_state_ptr->scene_game_objects;

  ptr = ptr + (*count);
  (*ptr) = game_object;

  (*count)++;
}

void cl_print_game_objects_on_scene(struct game_state *game_state_ptr)
{
  int count = game_state_ptr->game_objects_count;
  struct game_object **objects = game_state_ptr->scene_game_objects;
  for (int i = 0; i < count; i++)
  {
    printf("name : %s\n", objects[i]->name);
    char client_uuid_str[37];
    uuid_unparse_lower(objects[i]->uuid, client_uuid_str);
    printf("uuid = %s\n", client_uuid_str);
  }
}

// Control frame rate
// https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidetimeexamples.html
Uint32 time_left(Uint32 next_time)
{
  Uint32 now = SDL_GetTicks();
  if (next_time <= now)
  {
    return 0;
  }

  return next_time - now;
}

// Startup menu logic

void cl_startup_menu_init(struct game_state *game_state)
{
  game_state->page = STARTUP_SCREEN_PAGE;
  game_state->current_menu = LOGIN_MENU_ITEM;

  game_state->startup_menu = SDL_LoadBMP("assets/startup_menu.bmp");
  game_state->underscore = SDL_LoadBMP("assets/underscore.bmp");
}

void cl_startup_menu_draw(struct game_state *game_state, struct player *player)
{

  draw_surface(0, 0, game_state->startup_menu, game_state->screen, NULL);

  char *username = player->username;
  char *password = player->password;

  if (game_state->current_menu == 0)
  {
    draw_surface(330, 535, game_state->underscore, game_state->screen, NULL);
  }

  if (game_state->current_menu == 1)
  {
    draw_surface(330, 570, game_state->underscore, game_state->screen, NULL);
  }

  if (game_state->current_menu == 2)
  {
    draw_surface(330, 695, game_state->underscore, game_state->screen, NULL);
  }

  if (username && strlen(username) > 0)
  {
    write_text(game_state->screen, username, 495, 495, 255, 255, 255);
  }

  if (password && strlen(password) > 0)
  {
    write_text(game_state->screen, password, 495, 528, 255, 255, 255);
  }
}

void cl_startup_menu_keyboard_handler(struct game_state *game_state, struct player *player, int x, int y, int dimension, char *key)
{
  char *username = player->username;
  char *password = player->password;

  if (x == 0 && y == 0 && dimension == 0)
  {
    // login
    if (game_state->current_menu == LOGIN_MENU_ITEM)
    {
      if (!username)
      {
        username = (char *)calloc(1, sizeof(char));
      }

      if (strncmp(key, "backspace", sizeof("backspace")) == 0)
      {
        username[strlen(username) - 1] = '\0';
      }
      else
      {
        strncat(username, key, 1);
      }
    }

    // password
    if (game_state->current_menu == PASSWORD_MENU_ITEM)
    {
      if (!password)
      {
        password = (char *)calloc(1, sizeof(char));
      }

      if (strncmp(key, "backspace", sizeof("backspace")) == 0)
      {
        password[strlen(password) - 1] = '\0';
      }
      else
      {
        strncat(password, key, 1);
      }
    }

    // start
    if (game_state->current_menu == START_MENU_ITEM)
    {
      if (strncmp(key, "return", sizeof("return")) == 0)
      {
        game_state->page = GAME_PAGE;
      }
    }

    return;
  }

  game_state->current_menu -= dimension;
  if (game_state->current_menu > 2 || game_state->current_menu < 0)
  {
    game_state->current_menu = 0;
  }
}