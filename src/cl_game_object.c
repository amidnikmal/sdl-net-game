#include "cl_game_object.h"

struct game_object cl_create_player_game_object(char *name)
{
  struct game_object game_object;

  // If not using malloc will be crash while copy string. Why ?
  game_object.name = (char *)malloc(strlen(name) + 1);
  strcpy(game_object.name, name);

  game_object.rect.h = 100;
  game_object.rect.w = 100;
  game_object.rect.x = 0;
  game_object.rect.y = 0;

  game_object.surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
  SDL_FillRect(
      game_object.surface,
      &game_object.rect,
      SDL_MapRGB(game_object.surface->format, 200, 0, 0));

  return game_object;
}