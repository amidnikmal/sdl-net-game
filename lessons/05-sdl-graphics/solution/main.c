#include <SDL/SDL.h>
#include <stdio.h>

static Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
    Uint8 *base = (Uint8 *)surface->pixels;
    Uint32 *row = (Uint32 *)(base + y * surface->pitch);
    return row[x];
}

int main(void)
{
    SDL_Surface *canvas = NULL;
    SDL_Surface *sprite = NULL;
    SDL_Rect dst_rect;
    Uint8 r = 0, g = 0, b = 0;
    Uint32 white;
    Uint32 red;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    canvas = SDL_CreateRGBSurface(SDL_SWSURFACE, 8, 6, 32, 0, 0, 0, 0);
    sprite = SDL_CreateRGBSurface(SDL_SWSURFACE, 3, 2, 32, 0, 0, 0, 0);
    if (!canvas || !sprite)
    {
        fprintf(stderr, "SDL_CreateRGBSurface failed: %s\n", SDL_GetError());
        if (sprite)
        {
            SDL_FreeSurface(sprite);
        }
        if (canvas)
        {
            SDL_FreeSurface(canvas);
        }
        SDL_Quit();
        return 1;
    }

    white = SDL_MapRGB(canvas->format, 255, 255, 255);
    red = SDL_MapRGB(sprite->format, 255, 0, 0);

    SDL_FillRect(canvas, NULL, white);
    SDL_FillRect(sprite, NULL, red);

    dst_rect.x = 2;
    dst_rect.y = 1;

    if (SDL_BlitSurface(sprite, NULL, canvas, &dst_rect) < 0)
    {
        fprintf(stderr, "SDL_BlitSurface failed: %s\n", SDL_GetError());
        SDL_FreeSurface(sprite);
        SDL_FreeSurface(canvas);
        SDL_Quit();
        return 1;
    }

    SDL_GetRGB(get_pixel32(canvas, 2, 1), canvas->format, &r, &g, &b);
    printf("inside: %u %u %u\n", r, g, b);

    SDL_GetRGB(get_pixel32(canvas, 0, 0), canvas->format, &r, &g, &b);
    printf("outside: %u %u %u\n", r, g, b);

    SDL_FreeSurface(sprite);
    SDL_FreeSurface(canvas);
    SDL_Quit();
    return 0;
}
