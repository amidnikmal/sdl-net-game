#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PACKET_SIZE 10
#define TAG_SIZE 4

struct position_packet
{
    char tag[4];
    uint8_t type;
    uint16_t x;
    uint16_t y;
    uint8_t dimension;
};

static void write_u16_be(uint8_t *dst, uint16_t value)
{
    dst[0] = (uint8_t)((value >> 8) & 0xff);
    dst[1] = (uint8_t)(value & 0xff);
}

static uint16_t read_u16_be(const uint8_t *src)
{
    return (uint16_t)(((uint16_t)src[0] << 8) | (uint16_t)src[1]);
}

static void build_position_packet(uint8_t *buffer, uint8_t type, uint16_t x, uint16_t y, uint8_t dimension)
{
    memset(buffer, 0, PACKET_SIZE);
    memcpy(buffer, "POS", 3);
    buffer[4] = type;
    write_u16_be(buffer + 5, x);
    write_u16_be(buffer + 7, y);
    buffer[9] = dimension;
}

static void parse_position_packet(const uint8_t *buffer, struct position_packet *packet)
{
    memset(packet, 0, sizeof(*packet));
    memcpy(packet->tag, buffer, TAG_SIZE);
    packet->type = buffer[4];
    packet->x = read_u16_be(buffer + 5);
    packet->y = read_u16_be(buffer + 7);
    packet->dimension = buffer[9];
}

static void print_hex(const uint8_t *buffer, size_t size)
{
    printf("hex:");
    for (size_t i = 0; i < size; i++)
    {
        printf(" %02x", buffer[i]);
    }
    printf("\n");
}

int main(void)
{
    uint8_t buffer[PACKET_SIZE];
    struct position_packet packet;

    build_position_packet(buffer, 3, 300, 700, 7);
    parse_position_packet(buffer, &packet);

    print_hex(buffer, PACKET_SIZE);
    printf("tag: %s\n", packet.tag);
    printf("type: %u\n", packet.type);
    printf("x: %u\n", packet.x);
    printf("y: %u\n", packet.y);
    printf("dimension: %u\n", packet.dimension);

    return 0;
}
