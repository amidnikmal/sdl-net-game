#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 9091
#define BUFFER_SIZE 64

int main(void)
{
    int server_sock = -1;
    int client_sock = -1;

    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct sockaddr_in reply_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    socklen_t reply_addr_len = sizeof(reply_addr);

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    memset(&reply_addr, 0, sizeof(reply_addr));

    // TODO: создай UDP socket для server_sock.
    // Ожидается: AF_INET + SOCK_DGRAM

    // TODO: создай UDP socket для client_sock.

    server_addr.sin_family = AF_INET;
    // TODO: задай loopback-адрес 127.0.0.1.
    // Можно через inet_addr("127.0.0.1")
    // TODO: задай порт через htons(SERVER_PORT)

    // TODO: привяжи server_sock к server_addr через bind()

    // TODO: отправь строку "ping" с client_sock на server_addr через sendto()

    ssize_t received = recvfrom(
        server_sock,
        buffer,
        BUFFER_SIZE - 1,
        0,
        (struct sockaddr *)&client_addr,
        &client_addr_len);
    if (received < 0)
    {
        perror("recvfrom server");
        return 1;
    }

    buffer[received] = '\0';
    printf("server received: %s\n", buffer);

    // TODO: отправь "pong" обратно на client_addr через sendto()

    received = recvfrom(
        client_sock,
        buffer,
        BUFFER_SIZE - 1,
        0,
        (struct sockaddr *)&reply_addr,
        &reply_addr_len);
    if (received < 0)
    {
        perror("recvfrom client");
        return 1;
    }

    buffer[received] = '\0';
    printf("client received: %s\n", buffer);

    close(server_sock);
    close(client_sock);
    return 0;
}
