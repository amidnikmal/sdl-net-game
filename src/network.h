
#ifndef __GAME_NETWORK_H__
#define __GAME_NETWORK_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cl_player.h"
#include "serv_state.h"

// Client has connected
#define CONNECTED 1

#define USERNAME_MAX_SIZE 64
#define PACKET_HEADER_SIZE 64
#define PACKET_BODY_SIZE 512

#define PACKET_SIZE (PACKET_HEADER_SIZE + PACKET_BODY_SIZE)

///////////////// PACKET TYPES /////////////////

#define CONNECT_PACKET_TAG "CONNECT_PACKET"
#define CONNECT_PACKET_ID 1
// Used by client to notify server about connection.
// But also used by server to notify all players about new user has connected.
// ---------------------------------------------------------------------------------------------------------------
// | CONNECT_PACKET                                                                              |               |
// ---------------------------------------------------------------------------------------------------------------
// | CONNECT_PACKET_TAG (up to 64) | username (up to 64) | UUID (16) | x_coord (2) | y_coord (2) | dimension (2) |
// ---------------------------------------------------------------------------------------------------------------

#define DISCONNECT_PACKET_TAG "DISCONNECT_PACKET"
#define DISCONNECT_PACKET_ID 2
// --------------------------------------------
// | DISCONNECT_PACKET                        |
// --------------------------------------------
// | DISCONNECT_PACKET (up to 64) | UUID (16) |
// --------------------------------------------

#define POSITION_PACKET_TAG "POSITION_PACKET"
#define POSITION_PACKET_ID 3
// ------------------------------------------------------------------------------------------
// | POSITION_PACKET                                                                        |
// ------------------------------------------------------------------------------------------
// | POSITION_PACKET_TAG (up to 64) | UUID (16) | x_coord (2) | y_coord (2) | dimension (2) |
// ------------------------------------------------------------------------------------------

#define SHOT_PACKET_TAG "SHOT_PACKET"
#define SHOT_PACKET_ID 4
// ----------------------------------------------------------------------
// | SHOT_PACKET                                                        |
// ----------------------------------------------------------------------
// | SHOT_PACKET_TAG (up to 64) | UUID (16) | x_coord (2) | y_coord (2) |
// ----------------------------------------------------------------------

//////////// Send packet
void send_packet(int socket, struct sockaddr_in *addr, char *packet);

//////////// Packet builders
char *build_conn_packet(struct player *p);
char *build_position_packet(struct player *p);

//////////// Utils
uint8_t convert_packet_tag_to_id(char *packet_tag);
char *convert_packet_id_to_tag(uint8_t packet_id);
void print_addr_info(struct sockaddr_in *addr);
void print_uuid(uuid_t uuid, char *msg);

#endif