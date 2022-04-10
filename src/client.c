#include "cl_graph.h"
#include "cl_game_object.h"
#include "cl_game_state.h"
#include "cl_player.h"
#include "cl_events.h"
#include "cl_network.h"

#include "network.h"

#include <stdint.h>

struct game_state game_state;
struct player *player;

void clean_game(void);
void on_change_player(int x, int y, int dimension, char *key);

int main(int argc, char **argv)
{
  init_graph(&game_state.screen);

  game_state.background = SDL_LoadBMP("assets/background.bmp");
  ttf_init("assets/THWACK.TTF");

  player = create_new_player("", &game_state);
  game_state.player = player;

  cl_init_conn_to_server(player, 8888);

  game_state.game_running = 1;
  game_state.next_time = SDL_GetTicks() + TICK_INTERVAL;

  cl_startup_menu_init(&game_state);

  while (game_state.game_running)
  {

    if (game_state.page == STARTUP_SCREEN_PAGE)
    {
      cl_startup_menu_draw(&game_state, player);
    }

    if (game_state.page == GAME_PAGE)
    {
      cl_read_packets(player);
      cl_send_packets(player);

      draw_surface(0, 0, game_state.background, game_state.screen, NULL);
      draw_players(&game_state);
    }

    event_handle(&game_state.event, clean_game, on_change_player);

    SDL_Flip(game_state.screen);
    SDL_Delay(time_left(game_state.next_time));

    game_state.next_time += TICK_INTERVAL;
  }

  return 0;
}

void clean_game(void)
{
  graph_clean(game_state.screen);
  game_state.game_running = 0;
  exit(0);
}

void on_change_player(int x, int y, int dimension, char *key)
{
  if (game_state.page == GAME_PAGE)
  {
    player->x_coord += x;
    player->y_coord += y;
  }

  if (game_state.page == STARTUP_SCREEN_PAGE)
  {
    cl_startup_menu_keyboard_handler(&game_state, player, x, y, dimension, key);
  }
}