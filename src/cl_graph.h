#ifndef __GRAPH__
#define __GRAPH__

#include <SDL/SDL.h>

#define SCREEN_HEIGTH 1024
#define SCREEN_WIDTH 768
#define BPP 32 // bits per pixel

int init_graph(SDL_Surface **screen);
void graph_clean(SDL_Surface *screen);

SDL_Surface *load_image(const char *filename);

// load image excluding background
SDL_Surface *load_image_exc(const char *filename, int r, int g, int b);

void draw_surface(int x, int y, SDL_Surface *src, SDL_Surface *dst, SDL_Rect *clip);

#endif
