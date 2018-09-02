#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include "logger.h"
#include "udp.h"
#include "udplib.h"
#include "gs_api.h"
#include "gserv.h"

extern Logger *LOG;

item_t obj[MAXOBJ];
int objcnt;

gp_color_t bgcolor;


void* process_udp_message(void *buf, size_t n) {
   u_int8_t *set;
   gp_header_t *hd;
   gp_color_t *color;
   gp_point_t *point;
   gp_line_t *line;
   int cnt = 0;

   set = (u_int8_t *)buf;
   printf("Received: set:%u\n",*set);
   cnt++;

   do {

		hd = (gp_header_t *)(buf+cnt);
	   cnt += sizeof(hd);

		switch(hd->type) {

		   case GP_CLEAR  :
		   	objcnt = 0;
		      break;

		   case GP_COLOR  :

				color = (gp_color_t *)(buf+cnt);
			   cnt += sizeof(color);

			   obj[objcnt].s = *set;
			   obj[objcnt].t = hd->type;
			   obj[objcnt].d.color.r = color->r;
			   obj[objcnt].d.color.g = color->g;
			   obj[objcnt].d.color.b = color->b;
			   obj[objcnt].d.color.alpha = color->alpha;
			   objcnt++;

		      break;

		   case GP_POINT  :

		   	while (hd->count>0) {
					point = (gp_point_t *)(buf+cnt);
				   cnt += sizeof(point);

				   obj[objcnt].s = *set;
				   obj[objcnt].t = hd->type;
				   obj[objcnt].d.point.x = point->x;
				   obj[objcnt].d.point.y = point->y;
				   objcnt++;

				   hd->count--;
				}

		      break;
				  
		   case GP_LINE  :

		   	while (hd->count>0) {
					line = (gp_line_t *)(buf+cnt);
				   cnt += sizeof(line);

				   obj[objcnt].s = *set;
				   obj[objcnt].t = hd->type;
				   obj[objcnt].d.line.x1 = line->x1;
				   obj[objcnt].d.line.y1 = line->y1;
				   obj[objcnt].d.line.x2 = line->x2;
				   obj[objcnt].d.line.y2 = line->y2;
				   objcnt++;

				   hd->count--;
				}

		      break;
				  
		   case GP_SCRBGCOLOR  :

				color = (gp_color_t *)(buf+cnt);
			   cnt += sizeof(color);

				bgcolor.r = color->r;
				bgcolor.g = color->g;
				bgcolor.b = color->b;
				bgcolor.alpha = color->alpha;

		      break;

		}   

   } while (hd->type != GP_END);

   return EXIT_SUCCESS;
}


void* udp_listener_loop(void *arg) {
   if ( udp_server(&process_udp_message,UDP_PORT,1) ) {
      log_error(LOG, "Error starting udp_listener. Port %d busy.", UDP_PORT);
      exit(0);
   }
   return 0;
}


