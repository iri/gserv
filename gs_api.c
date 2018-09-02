#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include "gs_api.h"
#include "udplib.h"

int send_udp_binary(char const *msg, int l, char *addr, int port);


int gs_set_begin(char* buf, u_int8_t id, int* i) {
   *buf = id;
   *i = 1;
   return 0;
}

int gs_point_set(char* buf, int cnt, int* i) {
   static gp_header_t *hd;
   hd = (gp_header_t *)(buf + *i);
   hd->type = GP_POINT;
   hd->count = cnt;
   *i +=sizeof(hd);
   return 0;
}

int gs_point(char* buf, u_int16_t x, u_int16_t y, int* i) {
   static gp_point_t *point;
   point = (gp_point_t *)(buf + *i);
   point->x = x;
   point->y = y;
   *i +=sizeof(point);
   return 0;
}

int gs_line_set(char* buf, int cnt, int* i) {
   static gp_header_t *hd;
   hd = (gp_header_t *)(buf + *i);
   hd->type = GP_LINE;
   hd->count = cnt;
   *i +=sizeof(hd);
   return 0;
}

int gs_line(char* buf, u_int16_t x1, u_int16_t y1, u_int16_t x2, u_int16_t y2, int* i) {
   static gp_line_t *line;
   line = (gp_line_t *)(buf + *i);
   line->x1 = x1;
   line->y1 = y1;
   line->x2 = x2;
   line->y2 = y2;
   *i +=sizeof(line);
   return 0;
}

int gs_color(char* buf, u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t alpha, int* i) {
   static gp_header_t *hd;
   static gp_color_t *color;

   hd = (gp_header_t *)(buf + *i);
   hd->type = GP_COLOR;
   hd->count = 1;
   *i +=sizeof(hd);

   color = (gp_color_t *)(buf + *i);
   color->r = r;
   color->g = g;
   color->b = b;
   color->alpha = alpha;
   *i +=sizeof(color);

   return 0;
}

int gs_scr_bgcolor(char* buf, u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t alpha, int* i) {
   static gp_header_t *hd;
   static gp_color_t *color;

   hd = (gp_header_t *)(buf + *i);
   hd->type = GP_SCRBGCOLOR;
   hd->count = 1;
   *i +=sizeof(hd);

   color = (gp_color_t *)(buf + *i);
   color->r = r;
   color->g = g;
   color->b = b;
   color->alpha = alpha;
   *i +=sizeof(color);

   return 0;
}

int gs_scr_clr(char* buf, int* i) {
   static gp_header_t *hd;
   hd = (gp_header_t *)(buf + *i);
   hd->type = GP_CLEAR;
   hd->count = 0;
   *i +=sizeof(hd);
   return 0;
}

int gs_set_end(char* buf, int* i) {
   static gp_header_t *hd;
   hd = (gp_header_t *)(buf + *i);
   hd->type = GP_END;
   hd->count = 0;
   *i +=sizeof(hd);
   return 0;
}

int gs_set_send(char* buf, int l, char *addr, int port) {
   send_udp_binary(buf, l, addr, port);
   return 0;
}


int send_udp_binary(char const *msg, int l, char *addr, int port)
{
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        printf("send_udp_binary: create socket error\n" );
        exit(EXIT_FAILURE);
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);

    if (inet_aton(addr , &si_other.sin_addr) == 0)
    {
        printf("send_udp_binary: inet_aton() failed\n" );
        exit(EXIT_FAILURE);
    }

    //send the message
    if (sendto(s, msg, l, 0, (struct sockaddr *) &si_other, slen)==-1)
    {
        printf("send_udp_binary: sendto() failed\n" );
        exit(EXIT_FAILURE);
    } 

    close(s);
    return 0;
}

