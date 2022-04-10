#ifndef __CL_NETWORK_H__
#define __CL_NETWORK_H__

#include "network.h"
#include "cl_game_object.h"

void cl_send_packets(struct player *p);
void cl_read_packets(struct player *p);
void cl_init_conn_to_server(struct player *p, int server_port);
void cl_conn_to_server(struct player *p);
void cl_handle_conn_packet(struct player *p, char *packet);
struct player *cl_parse_connection_packet(char *packet);
void cl_handle_position_packet(struct player *p, char *packet);

#endif