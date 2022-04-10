
#ifndef __SERV_NETWORK_H__
#define __SERV_NETWORK_H__

#include "network.h"

//////////// Server packet parsers
void serv_parse_packet(struct serv_state *serv_state, char *packet, struct sockaddr_in *cliaddr);
void serv_handle_connection_packet(struct serv_state **serv_state, char *packet, struct sockaddr_in *cliaddr);
void serv_handle_position_packet(struct serv_state **serv_state, char *packet, struct sockaddr_in *cliaddr);

#endif