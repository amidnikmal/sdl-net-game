#include <stdio.h>
#include <string.h>

struct frame_input
{
    const char *net;
    const char *input;
};

static void apply_net_event(const char *net, int *x)
{
    // TODO: если net == "left", уменьши *x на 1.
    // Если net == "right", увеличь *x на 1.
    (void)net;
    (void)x;
}

static int apply_local_input(const char *input, int *x)
{
    // TODO: если input == "right", увеличь *x на 1 и верни 1.
    // TODO: если input == "left", уменьши *x на 1 и верни 1.
    // TODO: если input == "up" или "none", не меняй x и верни 0.
    (void)input;
    (void)x;
    return 0;
}

int main(void)
{
    struct frame_input frames[] = {
        {"none", "right"},
        {"left", "none"},
        {"none", "up"},
    };

    int x = 0;

    for (int i = 0; i < 3; i++)
    {
        int local_changed = 0;

        // TODO: сначала примени сетевое событие.
        // TODO: потом примени локальный input и запомни, изменил ли он x.

        printf(
            "frame %d: net=%s input=%s update=x=%d send=%s render=x=%d\n",
            i + 1,
            frames[i].net,
            frames[i].input,
            x,
            local_changed ? "yes" : "no",
            x);
    }

    return 0;
}
