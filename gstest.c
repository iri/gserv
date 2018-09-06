#include <sys/types.h>
#include "gs_api.h"

#define UDP_ADDR        "127.0.0.1"
#define UDP_PORT        7979

int main(int argc, char *argv[]) {
   char b[MAX_PACKET_LEN];
   int len,x=0;

   while (1) {

      // Form some test commands (sent as one UDP packet)
      len = 0;                                              // Init UDP packet length 
      gs_set_begin(b, 50, &len);                            // Init a command set
         gs_scr_clr(b, &len);                               // Clear screen
         gs_scr_bgcolor(b, 0x11, 0x11, 0x11, 0xFF, &len);   // Set background color
         gs_color(b, 0xFF, 0x00, 0x00, 0xFF, &len);         // Set drawing color (red)
         gs_point_set(b, 6, &len);                          // Init point set (6 points)
            gs_point(b, 200, 200, &len);                    //   Draw point
            gs_point(b, 201, 200, &len);                    //   Draw point
            gs_point(b, 202, 200, &len);                    //   Draw point
            gs_point(b, 203, 200, &len);                    //   Draw point
            gs_point(b, 300, 200, &len);                    //   Draw point
            gs_point(b, 301, 200, &len);                    //   Draw point
         gs_color(b, 0x50, 0xA0, 0xD0, 0xFF, &len);         // Set drawing color (green)
         gs_point_set(b, 16, &len);                          // Init point set (4 points)
            gs_point(b, 200, 220, &len);                    //   Draw point
            gs_point(b, 201, 220, &len);                    //   Draw point
            gs_point(b, 202, 220, &len);                    //   Draw point
            gs_point(b, 203, 220, &len);                    //   Draw point
            gs_point(b, 200, 221, &len);                    //   Draw point
            gs_point(b, 201, 221, &len);                    //   Draw point
            gs_point(b, 202, 221, &len);                    //   Draw point
            gs_point(b, 203, 221, &len);                    //   Draw point
            gs_point(b, 200, 222, &len);                    //   Draw point
            gs_point(b, 201, 222, &len);                    //   Draw point
            gs_point(b, 202, 222, &len);                    //   Draw point
            gs_point(b, 203, 222, &len);                    //   Draw point
            gs_point(b, 200, 223, &len);                    //   Draw point
            gs_point(b, 201, 223, &len);                    //   Draw point
            gs_point(b, 202, 223, &len);                    //   Draw point
            gs_point(b, 203, 223, &len);                    //   Draw point
         gs_color(b, 0xFF, 0xFF, 0xFF, 0xFF, &len);         // Set drawing color (white)
         gs_line_set(b, 2, &len);                           // Init line set (2 lines)
            gs_line(b, 200, 240, 400, 400, &len);           //   Draw line
            gs_line(b, 300, 270, 300, 400, &len);           //   Draw line
         gs_color(b, 0x00, 0x00, 0xFF, 0xFF, &len);         // Set drawing color (blue)
         gs_line_set(b, 4, &len);                           // Init line set (4 lines)
            gs_line(b, 400+x, 100, 400+x, 140, &len);       //   Draw line
            gs_line(b, 400+x, 140, 440+x, 140, &len);       //   Draw line
            gs_line(b, 440+x, 140, 440+x, 100, &len);       //   Draw line
            gs_line(b, 440+x, 100, 400+x, 100, &len);       //   Draw line
      gs_set_end(b, &len);                                  // Finish the command set

      gs_set_send(b, len, UDP_ADDR, UDP_PORT);              // Send the command set (as one UDP packet)

      x = (x+10) % 100;

      usleep(1000000);

   }

   return 0;
}

