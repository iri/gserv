#ifndef _GS_API_H_
#define _GS_API_H_

#include <unistd.h>

#define MAX_PACKET_LEN 	1450

// Data structures
struct gp_header {
	u_int8_t      type;
	u_int16_t     count;
} __attribute__((packed));
typedef struct gp_header gp_header_t;

struct gp_point {
	u_int16_t     x;
	u_int16_t     y;
} __attribute__((packed));
typedef struct gp_point gp_point_t;

struct gp_line {
	u_int16_t     x1;
	u_int16_t     y1;
	u_int16_t     x2;
	u_int16_t     y2;
} __attribute__((packed));
typedef struct gp_line gp_line_t;

struct gp_color {
	u_int8_t     r;
	u_int8_t     g;
	u_int8_t     b;
	u_int8_t     alpha;
} __attribute__((packed));
typedef struct gp_color gp_color_t;

union gp_item {
	gp_point_t point;
	gp_line_t line;
	gp_color_t color;
};
typedef union gp_item gp_item_t;


struct item {
	u_int8_t     s;		// set
	u_int8_t     t;		// type
	gp_item_t    d;		// data
} __attribute__((packed));
typedef struct item item_t;



#define GP_CLEAR        1
#define GP_POINT        2
#define GP_LINE         3
#define GP_COLOR        4
#define GP_SCRBGCOLOR   5
#define GP_END          255


/**
 ** UDP Packet payload structure
 **
 **   u_int8_t set
 **     u_int8_t type
 **     u_int16_t count
 **     gp_<type>_t items[count]
 **       ...
 **     u_int8_t type
 **     u_int16_t count
 **     gp_<type>_t items[count]
 **     u_int8_t 255
 **
 **/


int gs_set_begin(char* buf, u_int8_t id, int* i);
int gs_point_set(char* buf, int cnt, int* i);
int gs_point(char* buf, u_int16_t x, u_int16_t y, int* i);
int gs_line_set(char* buf, int cnt, int* i);
int gs_line(char* buf, u_int16_t x1, u_int16_t y1, u_int16_t x2, u_int16_t y2, int* i);
int gs_color(char* buf, u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t alpha, int* i);
int gs_scr_bgcolor(char* buf, u_int8_t r, u_int8_t g, u_int8_t b, u_int8_t alpha, int* i);
int gs_scr_clr(char* buf, int* i);
int gs_set_end(char* buf, int* i);
int gs_set_send(char* buf, int l, char *addr, int port);


#endif

