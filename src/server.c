
#include <stdlib.h>
#include <unistd.h>

#include "network.h"
#include "serv_state.h"
#include "serv_network.h"
#include <sys/epoll.h>

#define MAX_EVENTS 10
#define PORT 8888

struct serv_state serv_state;
int epollfd;
int nfds;

struct epoll_event ev, events[MAX_EVENTS];

int main(int argc, char **argv)
{

  /*
    struct sockaddr {
        unsigned short    sa_family;    // address family, AF_xxx
        char              sa_data[14];  // 14 bytes of protocol address
    };

    Structure for holding socket address.
    It is more convenient to use structure `struct sockaddr_in`.

    struct sockaddr_in {
      short int          sin_family;  // Address family
      unsigned short int sin_port;    // Port number
      struct in_addr     sin_addr;    // Internet address
      unsigned char      sin_zero[8]; // Same size as struct sockaddr
    };

    `in` - means "Internet"

    With help of that structure more easily to deal with address section.

    `sin_zero` - should be filled with zeros to align with `sockaddr` structure

    `sockaddr_in` - can be easily converted to sockaddr and vice versa.
  */

  /* AF_INET - Interent SOCK_DGRAM - Using UDP Protocol */

  if ((serv_state.socket = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("Socket creation error");
    exit(1);
  }

  serv_state.servaddr.sin_family = AF_INET;                // Server is in Internet Domain
  serv_state.servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Server's Internet Address
  serv_state.servaddr.sin_port = htons(PORT);              // Host to network short (Network byte order)

  socklen_t size = sizeof(serv_state.servaddr);

  if (bind(serv_state.socket, (struct sockaddr *)&(serv_state.servaddr), size) < 0)
  {
    perror("Bind servaddr error");
    exit(2);
  }



  epollfd = epoll_create1(0);
  if (epollfd == -1) {  
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }




  ev.events = EPOLLIN;
  ev.data.fd = serv_state.socket;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, serv_state.socket, &ev) == -1) {
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
  }



  char buf[PACKET_SIZE];




  while (1)
  {


    // nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    // if (nfds == -1) {
    //   perror("epoll_wait");
    //   exit(EXIT_FAILURE);
    // }



    struct sockaddr_in cliaddr;

    ssize_t received = recvfrom(serv_state.socket, buf, PACKET_SIZE, 0, (struct sockaddr *)&cliaddr, &size);

    if (received < 0)
    {
      perror("recvfrom()");
      exit(4);
    }

    print_addr_info(&cliaddr);

    serv_parse_packet(&serv_state, buf, &cliaddr);
  }

  close(serv_state.socket);
}
