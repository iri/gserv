/* UDP server functions (for Linux/OSX/Windows) */

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_LINUX    3

#if defined(_WIN32)

    #define PLATFORM PLATFORM_WINDOWS

#elif defined(__APPLE__)

    #define PLATFORM PLATFORM_MAC

#else

    #define PLATFORM PLATFORM_LINUX

#endif

#if PLATFORM == PLATFORM_WINDOWS

    #include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_LINUX

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>
    #include <arpa/inet.h>

#endif

#if PLATFORM == PLATFORM_WINDOWS

    #pragma comment(lib,"ws2_32.lib")

#endif

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <sys/ioctl.h>
#include <net/if.h>

#include "logger.h"
#include "udplib.h"

extern Logger *LOG;

int started = 0;


int send_udp_socket;
struct sockaddr_in si_other;
int slen = sizeof(si_other);

int send_udp_init(char *addr, int port)
{
    int s;
    if ( (s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        log_error(LOG, "send_udp_init: create socket error");
        exit(EXIT_FAILURE);
    }

    memset((char *) &si_other, 0, slen);
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);

    if (inet_aton(addr , &si_other.sin_addr) == 0)
    {
        log_error(LOG, "send_udp_init: inet_aton() failed");
        exit(EXIT_FAILURE);
    }

    return s;
}


int send_udp(int s, char const *msg, int l)
{
    // Send the message
    if (sendto(s, msg, l, 0, (struct sockaddr *) &si_other, slen)==-1)
    {
        log_error(LOG, "send_udp: sendto() failed");
        exit(EXIT_FAILURE);
    }
    return 0;
}


int send_udp_binary(char const *msg, int l, char *addr, int port)
{
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        log_error(LOG, "send_udp_binary: create socket error");
        exit(EXIT_FAILURE);
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);

    if (inet_aton(addr , &si_other.sin_addr) == 0)
    {
        log_error(LOG, "send_udp_binary: inet_aton() failed");
        exit(EXIT_FAILURE);
    }

    //send the message
    if (sendto(s, msg, l, 0, (struct sockaddr *) &si_other, slen)==-1)
    {
        log_error(LOG, "send_udp_binary: sendto() failed");
        exit(EXIT_FAILURE);
    } 

    log_debug(LOG, "send_udp_binary: %s (%d bytes) sent by UDP to %s:%d", msg,l,addr,port);

    close(s);
    return 0;
}


int send_udp_ascii(char const *msg, char *addr, int port)
{
    return send_udp_binary(msg, strlen(msg), addr, port);
}


// get IP address for specific ethernet device
char* _ip(char *ethdevice)
{
   int fd;
   struct ifreq ifr;
   static char ip[20];

   fd = socket(AF_INET, SOCK_DGRAM, 0);

   ifr.ifr_addr.sa_family = AF_INET;
   strncpy(ifr.ifr_name, ethdevice, IFNAMSIZ-1);

   ioctl(fd, SIOCGIFADDR, &ifr);

   close(fd);

   strcpy(ip,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

   return ip;
}
// get IP address on OSX or Linux
char* ip()
{
   static char ip[20];
   static char *ip1,*ip2;
 
   ip1 = _ip((char*)"eth0");   // Linux device name
   ip2 = _ip((char*)"en1");    // OSX device name

   if ( strcmp(ip1,"0.0.0.0") ) {
      strcpy(ip,ip2);
   } else {
      strcpy(ip,ip1);
   }

   return ip;
}

uint32_t parseIPV4string(char const * ipAddress)
{
   unsigned int buf[4];
   if ( 4 != sscanf(ipAddress, "%u.%u.%u.%u", &buf[0], &buf[1], &buf[2], &buf[3]) )
      return 0;   // or some other indicator or error
   return (buf[0]<<24)+(buf[1]<<16)+(buf[2]<<8)+buf[3];
}

char* IPV4tostring(uint32_t ip)
{
   static char ipstr[20];
   sprintf(ipstr, "%d.%d.%d.%d", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, ip & 0xFF);        
   return ipstr;
}

unsigned long client_port = 0;     // actual client port id

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_LINUX
    void macLinuxUDPLoop(int, struct sockaddr*, socklen_t, void *(*process_udp_message) (void *, size_t) );
    int macLinuxUDPServer( void *(*process_udp_message) (void *, size_t), int, int );
#endif

#if PLATFORM == PLATFORM_WINDOWS
    void windowsUDPLoop(SOCKET, sockaddr*, size_t, void *(*process_udp_message) (void *, size_t) );
    int windowsUDPServer( void *(*process_udp_message) (void *, size_t), int, int );
#endif


int udp_server(void *(*process_udp_message) (void *, size_t), int port, int port_fixed)
{
    #if PLATFORM == PLATFORM_WINDOWS
        return windowsUDPServer(process_udp_message, port, port_fixed);
    #else
        return macLinuxUDPServer(process_udp_message, port, port_fixed);
    #endif
    return EXIT_SUCCESS;
}

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_LINUX

int macLinuxUDPServer( void *(*process_udp_message) (void *, size_t), int port, int port_fixed )
{
    int socketFileDescriptor;
    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;
    int r;

    socketFileDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    bzero(&serverAddr, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    r = -1;
    client_port = port;
    if (port_fixed==1) {
        serverAddr.sin_port = htons(client_port);
        r = bind(socketFileDescriptor, (struct sockaddr*)& serverAddr, sizeof(serverAddr));
        if (r<0) {
            return EXIT_FAILURE;
        }
    } else {
        while (r<0) {
            serverAddr.sin_port = htons(client_port++);
            r = bind(socketFileDescriptor, (struct sockaddr*)& serverAddr, sizeof(serverAddr));
        }
        client_port--;
    }

    started = 1;

    macLinuxUDPLoop(socketFileDescriptor, (struct sockaddr*)& clientAddr, sizeof(clientAddr),process_udp_message);

    return EXIT_SUCCESS;
}

void macLinuxUDPLoop(int sockFd, struct sockaddr* cliaddr, socklen_t clilen, void *(*process_udp_message) (void *, size_t) )
{
    int bytesRead;
    socklen_t len;
    char msg[UDP_BUFMAX] = {0};

    for(;;)
    {
        len = clilen;
        bzero(&msg, sizeof(msg));
        bytesRead = recvfrom(sockFd, msg, UDP_BUFMAX, 0, cliaddr, &len);
        (*process_udp_message)(msg, bytesRead);
    }
}

#endif

#if PLATFORM == PLATFORM_WINDOWS

int windowsUDPServer( void *(*process_udp_message) (void *, size_t), int port, int port_fixed )
{
    SOCKET sock;
    sockaddr_in serverAddr;
    sockaddr_in clientAddr;
    WSADATA wsaDat;
    int r;

    int wsaError = WSAStartup( MAKEWORD(2,2), &wsaDat );

    if(!wsaError)
    {
        sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        ZeroMemory(&serverAddr, sizeof(serverAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        r = -1;
        client_port = port;
        if (port_fixed==1) {
            serverAddr.sin_port = htons(client_port);
            r = bind(sock, (struct sockaddr*)& serverAddr, sizeof(serverAddr));
            if (r<0) {
            }
        } else {
            while (r<0) {
                serverAddr.sin_port = htons(client_port++);
                r = bind(sock, (struct sockaddr*)& serverAddr, sizeof(serverAddr));
            }
            client_port--;
        }

        started = 1;

        windowsUDPLoop(sock, (struct sockaddr*)& clientAddr, sizeof(clientAddr),process_udp_message);

        return EXIT_SUCCESS;
    }
    else
    {
        return EXIT_FAILURE;
    }
}

void windowsUDPLoop(SOCKET sock, sockaddr* cliaddr, size_t clilen, void *(*process_udp_message) (void *, size_t) )
{
    int bytesRead;
    int len;
    char msg[UDP_BUFMAX] = {0};

    for(;;)
    {
        len = clilen;
        ZeroMemory(&msg, sizeof(msg));
        bytesRead = recvfrom(sock, msg, UDP_BUFMAX, 0, cliaddr, &len);
        (*process_udp_message)(msg, bytesRead);
    }

}

#endif


