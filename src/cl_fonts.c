#include "cl_fonts.h"

TTF_Font *font = NULL;

void ttf_init(const char *filename)
{
  if (TTF_Init() < 0)
  {
    fprintf(stderr, "Couldn't initialize SDL TTF: %s\n", TTF_GetError());
    exit(1);
  }

  font = TTF_OpenFont(filename, 30);

  if (!font)
  {
    fprintf(stderr, "Unable to open font. %s\n", TTF_GetError());
    exit(1);
  }

  printf("fonts is initialized\n");
}

void ttf_clean()
{
  TTF_Quit();
}

int write_text(SDL_Surface *dst, const char *str, int x, int y, int r, int g, int b)
{
  SDL_Surface *text;
  SDL_Rect fontRect;
  SDL_Color text_color = {255, 255, 255};

  text = TTF_RenderUTF8_Blended(font, str, text_color);

  if (!text)
  {
    fprintf(stderr, "Unable to render text: %s; %s\n", str, TTF_GetError());
    return -1;
  }

  fontRect.x = x;
  fontRect.y = y;

  text_color.r = r;
  text_color.g = g;
  text_color.b = b;

  SDL_BlitSurface(text, NULL, dst, &fontRect);

  return 0;
}
