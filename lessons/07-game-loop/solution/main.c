#include <stdio.h>
#include <string.h>

struct frame_input
{
    const char *net;
    const char *input;
};

static void apply_net_event(const char *net, int *x)
{
    if (strcmp(net, "left") == 0)
    {
        *x -= 1;
    }
    else if (strcmp(net, "right") == 0)
    {
        *x += 1;
    }
}

static int apply_local_input(const char *input, int *x)
{
    if (strcmp(input, "right") == 0)
    {
        *x += 1;
        return 1;
    }

    if (strcmp(input, "left") == 0)
    {
        *x -= 1;
        return 1;
    }

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

        apply_net_event(frames[i].net, &x);
        local_changed = apply_local_input(frames[i].input, &x);

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
