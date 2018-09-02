#ifndef TIME_H
#define TIME_H

typedef struct timeval TIMER;
typedef unsigned long MICROSEC;

void timer_start(TIMER *t);
MICROSEC timer_stop(TIMER *t);

#endif

