#include "serv_network.h"

void serv_parse_packet(struct serv_state *serv_state, char *packet, struct sockaddr_in *cliaddr)
{
  char *header = (char *)malloc(PACKET_HEADER_SIZE);
  memcpy(header, packet, PACKET_HEADER_SIZE);

  uint8_t numerical_packet_type = convert_packet_tag_to_id(header);

  switch (numerical_packet_type)
  {
  case CONNECT_PACKET_ID:
  {
    serv_handle_connection_packet(&serv_state, packet, cliaddr);
    break;
  }
  case POSITION_PACKET_ID:
  {
    serv_handle_position_packet(&serv_state, packet, cliaddr);
    break;
  }

  default:
  {
  }
  }
}

void serv_handle_position_packet(struct serv_state **serv_state, char *packet, struct sockaddr_in *cliaddr)
{
  char *ptr = packet + PACKET_HEADER_SIZE;

  struct serv_state *serv_state_ptr = *serv_state;
  uint16_t *count = &(serv_state_ptr->players_count);

  uuid_t uuid;
  memcpy((char *)uuid, ptr, 16);
  print_uuid(uuid, "UUID:POSITION:PACKET");

  for (int i = 0; i < *count; i++)
  {
    struct player p = serv_state_ptr->players[i];
    if (uuid_compare(p.uuid, uuid) == 0)
    {
      continue;
    }
    send_packet(serv_state_ptr->socket, p.cliaddr, packet);
  }

  return;
}

void serv_handle_connection_packet(struct serv_state **serv_state, char *packet, struct sockaddr_in *cliaddr)
{
  char *ptr = packet + PACKET_HEADER_SIZE;
  char *body = (char *)malloc(PACKET_BODY_SIZE);
  memcpy(body, ptr, PACKET_BODY_SIZE);

  struct serv_state *serv_state_ptr = *serv_state;
  uint16_t *count = &(serv_state_ptr->players_count);

  if (*count > 0)
  {
    serv_state_ptr->players = (struct player *)realloc(serv_state_ptr->players, (*count + 1) * sizeof(struct player));
  }
  else
  {
    serv_state_ptr->players = (struct player *)malloc(sizeof(struct player));
  }

  struct player *new_player = serv_state_ptr->players + *count;
  char *packet_ptr = body;

  new_player->cliaddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
  memcpy(new_player->cliaddr, cliaddr, sizeof(struct sockaddr_in));

  new_player->username = (char *)malloc(USERNAME_MAX_SIZE);
  strncpy(new_player->username, packet_ptr, USERNAME_MAX_SIZE);

  packet_ptr = packet_ptr + USERNAME_MAX_SIZE;
  memcpy(new_player->uuid, packet_ptr, 16);

  packet_ptr = packet_ptr + 16;
  new_player->x_coord = *(int16_t *)packet_ptr;

  packet_ptr = packet_ptr + 2;
  new_player->y_coord = *(int16_t *)packet_ptr;

  packet_ptr = packet_ptr + 2;
  new_player->dimension = *(uint8_t *)packet_ptr;

  // send to newly connect user info about already connected users
  for (int i = 0; i < *count; i++)
  {
    struct player p = serv_state_ptr->players[i];
    send_packet(serv_state_ptr->socket, new_player->cliaddr, build_conn_packet(&p));
  }

  (*count)++;

  print_uuid(new_player->uuid, ">>> New player has connected:");
  printf(">>> New player has connected: %s\n", new_player->username);

  // broadcast to all players, that has already connected
  for (int i = 0; i < *count; i++)
  {
    struct player p = serv_state_ptr->players[i];
    print_uuid(p.uuid, "broadcast:conn:packet:>>>>>>>>>>>");
    send_packet(serv_state_ptr->socket, p.cliaddr, packet);
  }
}