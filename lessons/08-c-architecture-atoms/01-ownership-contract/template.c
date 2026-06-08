#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct packet
{
    char *data;
    size_t size;
};

static struct packet *packet_create(const char *text)
{
    // TODO:
    // 1. выдели struct packet
    // 2. выдели память под data
    // 3. скопируй text
    // 4. запиши size
    (void)text;
    return NULL;
}

static void packet_destroy(struct packet *packet)
{
    // TODO:
    // освободить data, потом сам packet
    (void)packet;
}

static void send_packet(const struct packet *packet)
{
    printf("text: %s\n", packet->data);
    printf("size: %zu\n", packet->size);
}

int main(void)
{
    struct packet *packet = packet_create("ping");
    if (!packet)
    {
        return 1;
    }

    send_packet(packet);
    packet_destroy(packet);
    return 0;
}
