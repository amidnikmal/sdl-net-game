#include <stdio.h>
#include <string.h>

struct app_context
{
    int ticks;
};

static void handle_command(struct app_context *ctx, const char *command)
{
    // TODO: если command == "tick", увеличь ctx->ticks на 1
    (void)ctx;
    (void)command;
}

static void run_frame(struct app_context *ctx, const char *command)
{
    // TODO: вызывай handle_command() через ctx
    (void)ctx;
    (void)command;
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
