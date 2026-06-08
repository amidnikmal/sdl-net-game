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
    dst[0] = 0;
    dst[1] = 0;
    (void)value;
}

static uint16_t read_u16_be(const uint8_t *src)
{
    (void)src;
    return 0;
}

static void encode_position_packet(uint8_t *buffer, const struct position *position)
{
    // TODO: type в buffer[0], x в buffer[1..2], y в buffer[3..4]
    (void)buffer;
    (void)position;
}

static void decode_position_packet(const uint8_t *buffer, struct position *position)
{
    // TODO: прочитай буфер обратно в position
    (void)buffer;
    (void)position;
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
