#include "cl_graph.h"

int init_graph(SDL_Surface **screen)
{

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("SDL_Init() error\n");
    return -1;
  }

  /* SW_SURFACE - surface in software memory */
  *screen = SDL_SetVideoMode(SCREEN_HEIGTH, SCREEN_WIDTH, BPP, SDL_SWSURFACE);

  if (!*screen)
  {
    printf("SDL_SetVideoMode() error;\tCouldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGTH, BPP, SDL_GetError());
    return -1;
  }

  SDL_WM_SetCaption("CLIENT", NULL);
  // SDL_EnableKeyRepeat(5, 0);

  printf("graphics is initialized\n");
  return 0;
}

void graph_clean(SDL_Surface *screen)
{
  SDL_FreeSurface(screen);
  SDL_Quit();
}

SDL_Surface *load_image_exc(const char *filename, int r, int g, int b)
{
  SDL_Surface *loadedImage = SDL_LoadBMP(filename);
  Uint32 colorkey = SDL_MapRGB(loadedImage->format, r, g, b);
  SDL_SetColorKey(loadedImage, SDL_SRCCOLORKEY, colorkey);

  return loadedImage;
}

void draw_surface(
    int x,
    int y,
    SDL_Surface *src,
    SDL_Surface *dst,
    SDL_Rect *clip)
{
  SDL_Rect dst_rect;

  dst_rect.x = x;
  dst_rect.y = y;

  if (SDL_BlitSurface(src, clip, dst, &dst_rect) < 0)
  {
    printf("Unable to SDL_BlitSurface\n");
  }
}
