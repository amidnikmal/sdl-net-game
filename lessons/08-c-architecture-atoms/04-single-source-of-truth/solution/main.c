#include <stdio.h>

struct entity
{
    int world_x;
    int world_y;
    int screen_x;
    int screen_y;
};

static void entity_set_position(struct entity *entity, int x, int y)
{
    entity->world_x = x;
    entity->world_y = y;
    entity->screen_x = x;
    entity->screen_y = y;
}

static void entity_move(struct entity *entity, int dx, int dy)
{
    entity_set_position(entity, entity->world_x + dx, entity->world_y + dy);
}

int main(void)
{
    struct entity entity = {0};

    entity_set_position(&entity, 2, 4);
    entity_move(&entity, 3, 3);

    printf("world: %d %d\n", entity.world_x, entity.world_y);
    printf("screen: %d %d\n", entity.screen_x, entity.screen_y);
    return 0;
}
