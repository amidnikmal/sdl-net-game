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
    int value = 0;
    int i = 0;

    if (!text[0])
    {
        return PARSE_EMPTY;
    }

    while (text[i] != '\0')
    {
        if (!isdigit((unsigned char)text[i]))
        {
            return PARSE_INVALID;
        }

        value = value * 10 + (text[i] - '0');
        i++;
    }

    if (value < 1 || value > 65535)
    {
        return PARSE_RANGE;
    }

    *out_port = value;
    return PARSE_OK;
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
