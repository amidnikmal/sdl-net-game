
#include "cl_network.h"
#include <fcntl.h>

int setnonblocking(int sockfd)
{
	if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK) ==
	    -1) {
		return -1;
	}
	return 0;
}

struct player *cl_parse_connection_packet(char *packet)
{
  char *packet_ptr = packet + PACKET_HEADER_SIZE;

  struct player *pl = (struct player *)malloc(sizeof(struct player));

  pl->username = (char *)malloc(USERNAME_MAX_SIZE);
  strncpy(pl->username, packet_ptr, USERNAME_MAX_SIZE);

  packet_ptr = packet_ptr + USERNAME_MAX_SIZE;
  memcpy(pl->uuid, packet_ptr, 16);

  print_uuid(pl->uuid, "cl_parse_connection_packet:");

  packet_ptr = packet_ptr + 16;
  pl->x_coord = *(int16_t *)packet_ptr;

  packet_ptr = packet_ptr + 2;
  pl->y_coord = *(int16_t *)packet_ptr;

  packet_ptr = packet_ptr + 2;
  pl->dimension = *(uint8_t *)packet_ptr;

  return pl;
}

void cl_read_packets(struct player *p)
{

  char buf[1024];

  // socklen_t server_addr_size = sizeof(struct sockaddr_in);

  // struct sockaddr_in *servaddr_in = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
  // memcpy(servaddr_in, p->servaddr, sizeof(struct sockaddr_in));
  // struct sockaddr *servaddr = (struct sockaddr *)servaddr_in;

  // recvfrom(
  //     p->cl_socket,
  //     buf,
  //     PACKET_SIZE,
  //     0,
  //     servaddr,
  //     &server_addr_size);

   recv(
      p->cl_socket,
      buf,
      PACKET_SIZE,
      0);

  char *header = (char *)malloc(PACKET_HEADER_SIZE);
  memcpy(header, buf, PACKET_HEADER_SIZE);

  if (strcmp(header, CONNECT_PACKET_TAG) == 0) {
    cl_handle_conn_packet(p, buf);
    return;
  }

  if (strcmp(header, POSITION_PACKET_TAG) == 0) {
    cl_handle_position_packet(p, buf);
  } 
}

void cl_handle_conn_packet(struct player *p, char *packet)
{
  struct game_state *game_state = p->game_state;
  struct player *new_player = cl_parse_connection_packet(packet);

  new_player->game_state = game_state;

  // Mark current client as connected and exit
  if (uuid_compare(new_player->uuid, game_state->player->uuid) == 0)
  {
    printf("Server accept connection!\n");
    game_state->player->connected_to_server = 1;
    return;
  }

  uint16_t *count = &(game_state->other_players_count);

  if (*count > 0)
  {
    game_state->other_players = (struct player *)realloc(game_state->other_players, (*count + 1) * sizeof(struct player));
  }
  else
  {
    game_state->other_players = (struct player *)malloc(sizeof(struct player));
  }

  struct game_object game_object = cl_create_player_game_object(new_player->username);

  new_player->game_object = (struct game_object *)malloc(sizeof(struct game_object));

  memcpy(new_player->game_object, &game_object, sizeof(struct game_object));

  memcpy(game_state->other_players + (*count), new_player, sizeof(struct player));

  (*count)++;
}

void cl_handle_position_packet(struct player *p, char *packet)
{
  struct game_state *game_state = p->game_state;
  for (int i = 0; i < game_state->other_players_count; i++)
  {
    struct player *pl = &game_state->other_players[i];

    char *ptr = packet + PACKET_HEADER_SIZE;
    uuid_t uuid;
    memcpy((char *)uuid, ptr, 16);

    if (uuid_compare(uuid, pl->uuid) == 0)
    {
      ptr = ptr + 16;
      pl->x_coord = *(int16_t *)ptr;

      ptr = ptr + 2;
      pl->y_coord = *(int16_t *)ptr;
    }
  }
}

void cl_init_conn_to_server(struct player *p, int server_port)
{
  int s;
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("Socket creation error");
    exit(1);
  }

  p->cl_socket = s;

  setnonblocking(p->cl_socket);

  p->servaddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));

  memset(p->servaddr, 0, sizeof(struct sockaddr_in));
  p->servaddr->sin_family = AF_INET; // Server is in Internet Domain (IPv4)
  // serveraddr->sin_addr.s_addr = server_addr == NULL ? INADDR_ANY : inet_addr(server_addr);

  p->servaddr->sin_addr.s_addr = INADDR_ANY;
  // serveraddr.sin_addr.s_addr = inet_addr("85.143.172.232");

  p->servaddr->sin_port = htons(server_port);

  cl_conn_to_server(p);
}

void cl_conn_to_server(struct player *p)
{
  char *packet = build_conn_packet(p);
  // print_addr_info(p->servaddr);
  send_packet(p->cl_socket, p->servaddr, packet);
}

void cl_send_packets(struct player *p)
{
  if ((p->prev_x_coord - p->x_coord != 0) || (p->prev_y_coord - p->y_coord != 0)) {  

    printf("pos:%d\n", p->prev_x_coord);
    printf("new pos:%d\n", p->x_coord);
    printf("cmp %s\n",(p->prev_x_coord - p->x_coord != 0) ? "true" : "false" );

    p->prev_x_coord = p->x_coord;
    p->prev_y_coord = p->y_coord;
    char *packet = build_position_packet(p);
    send_packet(p->cl_socket, p->servaddr, packet);
  }
}
