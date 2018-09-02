#ifndef UDP_H
#define UDP_H

#include <stdlib.h>

// For ingoing UDP packets
#define UDP_PORT	    7979

void* udp_listener_loop(void *arg);


#endif



