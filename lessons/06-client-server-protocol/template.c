#include <stdio.h>
#include <string.h>

#define MAX_PLAYERS 4

enum packet_kind
{
    PACKET_CONNECT = 1,
    PACKET_POSITION = 3
};

struct player
{
    int id;
    char name[16];
};

struct server_state
{
    struct player players[MAX_PLAYERS];
    int count;
};

static int find_player_index(const struct server_state *state, int player_id)
{
    // TODO: верни индекс игрока по id или -1, если игрок не найден.
    (void)state;
    (void)player_id;
    return -1;
}

static void add_player(struct server_state *state, int player_id, const char *name)
{
    // TODO: добавь игрока в конец массива state->players.
    // Не забудь увеличить state->count.
    (void)state;
    (void)player_id;
    (void)name;
}

static void handle_connect(struct server_state *state, int player_id, const char *name)
{
    int sync_count = state->count;

    // TODO: добавь игрока в состояние.

    printf(
        "connect %s: sync=%d broadcast=%d total=%d\n",
        name,
        sync_count,
        0,
        state->count);
}

static void handle_position(const struct server_state *state, int player_id)
{
    // TODO: найди игрока.
    // TODO: посчитай, скольким другим игрокам надо переслать POSITION.
    // Если игрок не найден, ничего не печатай.
    (void)state;
    (void)player_id;
}

int main(void)
{
    struct server_state state;
    memset(&state, 0, sizeof(state));

    handle_connect(&state, 1, "alice");
    handle_connect(&state, 2, "bob");
    handle_position(&state, 1);

    return 0;
}
