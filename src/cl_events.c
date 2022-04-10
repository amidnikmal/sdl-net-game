#include "cl_events.h"

void event_handle(
    SDL_Event *event,
    void (*const clean_game)(void),
    void (*const on_change)(int x, int y, int dimension, char *key))
{
  while (SDL_PollEvent(event))
  {
    if (event->type == SDL_KEYDOWN)
    {
      keys_handler(*event, on_change);
    }
    else if (event->type == SDL_QUIT)
    {
      clean_game();
    }
  }
}

void keys_handler(
    SDL_Event event,
    void (*const on_change)(int x, int y, int dimension, char *key))
{
  switch (event.type)
  {

  case SDL_KEYDOWN:
  {

    if (
        event.key.keysym.sym != SDLK_LEFT &&
        event.key.keysym.sym != SDLK_RIGHT &&
        event.key.keysym.sym != SDLK_UP &&
        event.key.keysym.sym != SDLK_DOWN &&
        event.key.keysym.sym != SDLK_ESCAPE)
    {
      char *key = SDL_GetKeyName(event.key.keysym.sym);
      on_change(0, 0, 0, key);
      return;
    }

    switch (event.key.keysym.sym)
    {
    case SDLK_LEFT:
      on_change(-STEP, 0, LEFT, NULL);
      break;
    case SDLK_RIGHT:
      on_change(STEP, 0, RIGHT, NULL);
      break;
    case SDLK_UP:
      on_change(0, -STEP, UP, NULL);
      break;
    case SDLK_DOWN:
      on_change(0, STEP, DOWN, NULL);
      break;
    case SDLK_ESCAPE:
    {
      SDL_Event esc_event;
      esc_event.type = SDL_QUIT;
      SDL_PushEvent(&esc_event);
      break;
    }
    default:
      break;
    }

    break;
  }
  }
}
