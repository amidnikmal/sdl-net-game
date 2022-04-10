#ifndef __FONTS__
#define __FONTS__
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

void ttf_init(const char *filename);
void ttf_clean();
int write_text(SDL_Surface *sdt, const char *text, int x, int y, int r, int g, int b);

#endif
