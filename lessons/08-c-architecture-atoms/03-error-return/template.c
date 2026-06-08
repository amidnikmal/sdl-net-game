#include <ctype.h>
#include <stdio.h>

enum parse_status
{
    PARSE_OK = 0,
    PARSE_EMPTY,
    PARSE_INVALID,
    PARSE_RANGE
};

static enum parse_status parse_port(const char *text, int *out_port)
{
    // TODO:
    // - если строка пустая -> PARSE_EMPTY
    // - если есть нецифровой символ -> PARSE_INVALID
    // - если число вне диапазона 1..65535 -> PARSE_RANGE
    // - при успехе записать порт в out_port и вернуть PARSE_OK
    (void)text;
    (void)out_port;
    return PARSE_INVALID;
}

int main(void)
{
    int port = 0;

    if (parse_port("8080", &port) == PARSE_OK)
    {
        printf("ok: %d\n", port);
    }

    if (parse_port("abc", &port) == PARSE_INVALID)
    {
        printf("err: invalid\n");
    }

    return 0;
}
