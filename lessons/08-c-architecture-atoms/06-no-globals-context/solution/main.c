#include <stdio.h>
#include <string.h>

struct app_context
{
    int ticks;
};

static void handle_command(struct app_context *ctx, const char *command)
{
    if (strcmp(command, "tick") == 0)
    {
        ctx->ticks += 1;
    }
}

static void run_frame(struct app_context *ctx, const char *command)
{
    handle_command(ctx, command);
}

int main(void)
{
    struct app_context ctx = {0};

    run_frame(&ctx, "tick");
    run_frame(&ctx, "tick");
    run_frame(&ctx, "tick");

    printf("ticks: %d\n", ctx.ticks);
    return 0;
}
