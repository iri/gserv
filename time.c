#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include "time.h"

void timer_start(TIMER *t) {
    gettimeofday(t, NULL);
}

MICROSEC timer_stop(TIMER *t) {
    static TIMER t2;
    gettimeofday(&t2, NULL);
    return t2.tv_sec * 1000000 + t2.tv_usec 
         - t->tv_sec * 1000000 - t->tv_usec;
}

