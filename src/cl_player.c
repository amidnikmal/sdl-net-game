
#include "cl_player.h"

void print_players(struct game_state *game_state_ptr)
{
  struct player *players = game_state_ptr->other_players;
  uint16_t *count = &game_state_ptr->other_players_count;

  for (int i = 0; i < *count; i++)
  {
    print_uuid(players[i].uuid, "print_players:");
  }
}

void draw_players(struct game_state *game_state_ptr)
{
  draw_player(game_state_ptr->player);

  struct player *players = game_state_ptr->other_players;
  uint16_t *count = &game_state_ptr->other_players_count;

  for (int i = 0; i < *count; i++)
  {
    struct player *p = &players[i];
    struct game_object *g_o = p->game_object;
    draw_surface(p->x_coord, p->y_coord, g_o->surface, game_state_ptr->screen, &g_o->rect);
    write_text(game_state_ptr->screen, p->username, p->x_coord, p->y_coord, 255, 255, 255);
  }
}

struct player *create_new_player(char *username, struct game_state *game_state_ptr)
{
  struct player *new_player = (struct player *)malloc(sizeof(struct player));

  new_player->game_state = game_state_ptr;

  uuid_generate_time_safe(new_player->uuid);
  set_player_username(new_player, username);

  new_player->x_coord = 0;
  new_player->y_coord = 0;
  new_player->dimension = 0;
  new_player->life = 100;

  struct game_object *game_object = (struct game_object *)malloc(sizeof(struct game_object));

  char uuid_str[37];

  uuid_copy(game_object->uuid, new_player->uuid);

  // uuid_generate_time_safe(game_object->uuid);
  uuid_unparse_lower(game_object->uuid, uuid_str);
  printf("\n\ncreate_new_player:uuid = %s\n", uuid_str);

  game_object->rect.h = 100;
  game_object->rect.w = 100;
  game_object->rect.x = 0;
  game_object->rect.y = 0;

  game_object->surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
  game_object->name = malloc(strlen(username) + 1); // dont forget '\0' char
  strcpy(game_object->name, username);

  SDL_FillRect(
      game_object->surface,
      &game_object->rect,
      SDL_MapRGB(game_object->surface->format, 200, 0, 0));

  add_game_object_on_scene(game_object, &game_state_ptr);

  new_player->game_object = game_object;

  return new_player;
}

void draw_player(struct player *p)
{
  struct game_state *g_s = p->game_state;
  struct game_object *g_o = p->game_object;

  draw_surface(p->x_coord, p->y_coord, g_o->surface, g_s->screen, &g_o->rect);
  write_text(g_s->screen, p->username, p->x_coord, p->y_coord, 255, 255, 255);
}

void set_player_username(struct player *player, char *username)
{
  player->username = (char *)malloc(strlen(username) + 1);
  strncpy(player->username, username, strlen(username) + 1);
}
