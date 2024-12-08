

#include "network.h"
#include <SDL/SDL.h>

char *build_conn_packet(struct player *p)
{
  if (strlen(p->username) > USERNAME_MAX_SIZE)
  {
    perror("User name must be less than 64 bytes\n");
    return NULL;
  }

  char *packet = (char *)malloc(PACKET_SIZE);

  //////////// CONNECT_PACKET_TAG (up to 64)
  char *ptr = packet;
  strcpy(ptr, CONNECT_PACKET_TAG);

  //////////// USERNAME (up to 64)
  ptr = ptr + PACKET_HEADER_SIZE;
  strcpy(ptr, p->username);

  //////////// UUID (16)
  ptr = ptr + USERNAME_MAX_SIZE;
  memcpy(ptr, (char *)p->uuid, 16);

  return packet;
}

char *build_position_packet(struct player *p)
{
  char *packet = (char *)malloc(PACKET_SIZE);

  //////////// POSITION_PACKET_TAG (64)
  char *ptr = packet;
  strcpy(ptr, POSITION_PACKET_TAG);

  //////////// UUID (16)
  ptr = ptr + USERNAME_MAX_SIZE;
  memcpy(ptr, (char *)p->uuid, 16);

  //////////// x_coord
  ptr = ptr + 16;
  *(int16_t *)ptr = p->x_coord;

  //////////// y_coord
  ptr = ptr + 2;
  *(int16_t *)ptr = p->y_coord;

  //////////// dimension
  ptr = ptr + 2;
  *(uint8_t *)ptr = p->dimension;

  return packet;
}

void send_packet(int socket, struct sockaddr_in *addr, char *packet)
{
  ssize_t transfered_bytes = sendto(socket,
                                    packet,
                                    PACKET_SIZE,
                                    0,
                                    (struct sockaddr *)addr,
                                    sizeof(struct sockaddr));
  if (transfered_bytes < 0)
  {
    perror("sendto()");
    exit(4);
  }
}


uint8_t convert_packet_tag_to_id(char *packet_tag)
{
  if (strcmp(packet_tag, CONNECT_PACKET_TAG) == 0)
  {
    return CONNECT_PACKET_ID;
  }

  if (strcmp(packet_tag, DISCONNECT_PACKET_TAG) == 0)
  {
    return DISCONNECT_PACKET_ID;
  }

  if (strcmp(packet_tag, POSITION_PACKET_TAG) == 0)
  {
    return POSITION_PACKET_ID;
  }

  if (strcmp(packet_tag, SHOT_PACKET_TAG) == 0)
  {
    return SHOT_PACKET_ID;
  }

  return -1;
}

void print_addr_info(struct sockaddr_in *addr)
{
  printf("domain: %s\n", (addr->sin_family == AF_INET ? "AF_INET" : "UNKNOWN"));
  printf("port: %d\n", ntohs(addr->sin_port));
  printf("internet address %s\n", inet_ntoa(addr->sin_addr));
}
