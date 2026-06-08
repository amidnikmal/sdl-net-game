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
    for (int i = 0; i < state->count; i++)
    {
        if (state->players[i].id == player_id)
        {
            return i;
        }
    }
    return -1;
}

static void add_player(struct server_state *state, int player_id, const char *name)
{
    struct player *slot = &state->players[state->count];
    slot->id = player_id;
    strncpy(slot->name, name, sizeof(slot->name) - 1);
    slot->name[sizeof(slot->name) - 1] = '\0';
    state->count++;
}

static void handle_connect(struct server_state *state, int player_id, const char *name)
{
    int sync_count = state->count;
    add_player(state, player_id, name);

    printf(
        "connect %s: sync=%d broadcast=%d total=%d\n",
        name,
        sync_count,
        state->count,
        state->count);
}

static void handle_position(const struct server_state *state, int player_id)
{
    int idx = find_player_index(state, player_id);
    if (idx < 0)
    {
        return;
    }

    printf("position %s: forward=%d\n", state->players[idx].name, state->count - 1);
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
