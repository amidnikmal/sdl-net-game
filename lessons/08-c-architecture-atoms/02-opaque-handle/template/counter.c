#include "counter.h"

#include <stdlib.h>

struct counter
{
    int value;
};

counter_t *counter_create(void)
{
    // TODO: выдели counter и инициализируй value = 0
    return NULL;
}

void counter_inc(counter_t *counter)
{
    // TODO: увеличь value на 1
    (void)counter;
}

int counter_get(const counter_t *counter)
{
    // TODO: верни value
    (void)counter;
    return 0;
}

void counter_destroy(counter_t *counter)
{
    free(counter);
}
