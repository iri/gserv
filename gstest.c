#include <unistd.h>
#include "gs_api.h"

#define UDP_ADDR        "127.0.0.1"
#define UDP_PORT        7979

int main(int argc, char *argv[]) {
   char b[MAX_PACKET_LEN];
   int len;

   len = 0;
   gs_set_begin(b, 50, &len);
      gs_scr_clr(b, &len);
      gs_scr_bgcolor(b, 0x11, 0x11, 0x11, 0xFF, &len);
      gs_color(b, 0xFF, 0x00, 0x00, 0xFF, &len);
      gs_point_set(b, 6, &len);
         gs_point(b, 200, 200, &len);
         gs_point(b, 201, 200, &len);
         gs_point(b, 202, 200, &len);
         gs_point(b, 203, 200, &len);
         gs_point(b, 300, 200, &len);
         gs_point(b, 301, 200, &len);
      gs_color(b, 0x00, 0xFF, 0x00, 0xFF, &len);
      gs_point_set(b, 4, &len);
         gs_point(b, 200, 220, &len);
         gs_point(b, 201, 220, &len);
         gs_point(b, 202, 220, &len);
         gs_point(b, 203, 220, &len);
      gs_color(b, 0xFF, 0xFF, 0xFF, 0xFF, &len);
      gs_line_set(b, 2, &len);
         gs_line(b, 200, 220, 400, 400, &len);
         gs_line(b, 300, 250, 300, 400, &len);
   gs_set_end(b, &len);

   gs_set_send(b, len, UDP_ADDR, UDP_PORT);

   return 0;
}

