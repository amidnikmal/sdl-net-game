#include "counter.h"

#include <stdlib.h>

struct counter
{
    int value;
};

counter_t *counter_create(void)
{
    counter_t *counter = malloc(sizeof(*counter));
    if (!counter)
    {
        return NULL;
    }

    counter->value = 0;
    return counter;
}

void counter_inc(counter_t *counter)
{
    counter->value += 1;
}

int counter_get(const counter_t *counter)
{
    return counter->value;
}

void counter_destroy(counter_t *counter)
{
    free(counter);
}
