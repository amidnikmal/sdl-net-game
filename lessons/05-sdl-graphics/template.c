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

    // TODO: инициализируй SDL через SDL_Init(SDL_INIT_VIDEO)
    // При ошибке выведи SDL_GetError() и заверши программу с кодом 1.

    // TODO: создай canvas 8x6, 32 bpp через SDL_CreateRGBSurface()
    // TODO: создай sprite 3x2, 32 bpp через SDL_CreateRGBSurface()

    // TODO: получи white и red через SDL_MapRGB()
    // white = (255, 255, 255), red = (255, 0, 0)

    // TODO: залей canvas белым
    // TODO: залей sprite красным

    dst_rect.x = 2;
    dst_rect.y = 1;

    // TODO: blit'ни sprite на canvas через SDL_BlitSurface()

    SDL_GetRGB(get_pixel32(canvas, 2, 1), canvas->format, &r, &g, &b);
    printf("inside: %u %u %u\n", r, g, b);

    SDL_GetRGB(get_pixel32(canvas, 0, 0), canvas->format, &r, &g, &b);
    printf("outside: %u %u %u\n", r, g, b);

    if (sprite)
    {
        SDL_FreeSurface(sprite);
    }
    if (canvas)
    {
        SDL_FreeSurface(canvas);
    }
    SDL_Quit();

    return 0;
}
