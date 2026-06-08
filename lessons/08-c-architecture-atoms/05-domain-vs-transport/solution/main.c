#include <stdint.h>
#include <stdio.h>

struct position
{
    uint8_t type;
    uint16_t x;
    uint16_t y;
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

static void encode_position_packet(uint8_t *buffer, const struct position *position)
{
    buffer[0] = position->type;
    write_u16_be(buffer + 1, position->x);
    write_u16_be(buffer + 3, position->y);
}

static void decode_position_packet(const uint8_t *buffer, struct position *position)
{
    position->type = buffer[0];
    position->x = read_u16_be(buffer + 1);
    position->y = read_u16_be(buffer + 3);
}

static void print_hex(const uint8_t *buffer, int size)
{
    printf("hex:");
    for (int i = 0; i < size; i++)
    {
        printf(" %02x", buffer[i]);
    }
    printf("\n");
}

int main(void)
{
    uint8_t buffer[5] = {0};
    struct position input = {3, 300, 700};
    struct position output = {0};

    encode_position_packet(buffer, &input);
    decode_position_packet(buffer, &output);

    print_hex(buffer, 5);
    printf("decoded: %u %u %u\n", output.type, output.x, output.y);
    return 0;
}
