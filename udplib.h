#ifndef UDPLIB_H
#define UDPLIB_H

#define UDP_BUFMAX	2000

#include <stdlib.h>
#include <stdint.h>


int send_udp_init(char *addr, int port);
int send_udp(int s, char const *msg, int l);

int send_udp_binary(char const *msg, int l, char *addr, int port);
int send_udp_ascii(char const *msg, char *addr, int port);
char* ip(void);
uint32_t parseIPV4string(char const * ipAddress);
char* IPV4tostring(uint32_t ip);
int udp_server(void *(*process_udp_message) (void *, size_t), int, int );

#endif



