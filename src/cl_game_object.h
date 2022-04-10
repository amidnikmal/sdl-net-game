
#ifndef __CL_GAME_OBJECT__
#define __CL_GAME_OBJECT__

#include <SDL/SDL.h>
#include <uuid/uuid.h>

struct game_object
{
    char *name;
    uuid_t uuid;
    SDL_Surface *surface;
    SDL_Rect rect;
};

struct game_object cl_create_player_game_object(char *name);



#endif