#ifndef __SERV_STATE_H__
#define __SERV_STATE_H__

#include <stdint.h>
#include "cl_player.h"

struct serv_state
{
    // Players on scene
    uint16_t players_count;
    struct player *players;

    struct sockaddr_in servaddr;
    int socket;
};

#endif