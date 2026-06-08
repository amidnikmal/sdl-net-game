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
    struct packet *packet = malloc(sizeof(*packet));
    if (!packet)
    {
        return NULL;
    }

    packet->size = strlen(text);
    packet->data = malloc(packet->size + 1);
    if (!packet->data)
    {
        free(packet);
        return NULL;
    }

    memcpy(packet->data, text, packet->size + 1);
    return packet;
}

static void packet_destroy(struct packet *packet)
{
    if (!packet)
    {
        return;
    }

    free(packet->data);
    free(packet);
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
