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
    // TODO: запиши value в dst[0] и dst[1] в big-endian порядке.
    dst[0] = 0;
    dst[1] = 0;
}

static uint16_t read_u16_be(const uint8_t *src)
{
    // TODO: собери uint16_t из двух байтов src[0], src[1] в big-endian порядке.
    return 0;
}

static void build_position_packet(uint8_t *buffer, uint8_t type, uint16_t x, uint16_t y, uint8_t dimension)
{
    memset(buffer, 0, PACKET_SIZE);

    // TODO: запиши tag "POS\0" в buffer[0..3]
    // TODO: запиши type в buffer[4]
    // TODO: запиши x в buffer[5..6] через write_u16_be()
    // TODO: запиши y в buffer[7..8] через write_u16_be()
    // TODO: запиши dimension в buffer[9]
}

static void parse_position_packet(const uint8_t *buffer, struct position_packet *packet)
{
    memset(packet, 0, sizeof(*packet));

    // TODO: прочитай tag из buffer[0..3]
    // TODO: прочитай type из buffer[4]
    // TODO: прочитай x из buffer[5..6] через read_u16_be()
    // TODO: прочитай y из buffer[7..8] через read_u16_be()
    // TODO: прочитай dimension из buffer[9]
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
