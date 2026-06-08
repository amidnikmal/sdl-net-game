#include <stdio.h>

#include "counter.h"

int main(void)
{
    counter_t *counter = counter_create();
    if (!counter)
    {
        return 1;
    }

    counter_inc(counter);
    counter_inc(counter);
    counter_inc(counter);

    printf("value: %d\n", counter_get(counter));
    counter_destroy(counter);
    return 0;
}
