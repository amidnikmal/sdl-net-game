#ifndef COUNTER_H
#define COUNTER_H

typedef struct counter counter_t;

counter_t *counter_create(void);
void counter_inc(counter_t *counter);
int counter_get(const counter_t *counter);
void counter_destroy(counter_t *counter);

#endif
