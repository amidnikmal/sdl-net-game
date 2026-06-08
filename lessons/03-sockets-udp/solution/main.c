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
    int server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    int client_sock = socket(AF_INET, SOCK_DGRAM, 0);

    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    struct sockaddr_in reply_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    socklen_t reply_addr_len = sizeof(reply_addr);

    if (server_sock < 0 || client_sock < 0)
    {
        perror("socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    memset(&reply_addr, 0, sizeof(reply_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        return 1;
    }

    if (sendto(client_sock, "ping", 4, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("sendto ping");
        return 1;
    }

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

    if (sendto(server_sock, "pong", 4, 0, (struct sockaddr *)&client_addr, client_addr_len) < 0)
    {
        perror("sendto pong");
        return 1;
    }

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
