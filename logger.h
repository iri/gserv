/**
 **
 ** Copyright (c) Daniel Johnston 
 ** 
 ** Permission is hereby granted, free of charge, to any person obtaining
 ** a copy of this software and associated documentation files (the
 ** "Software"), to deal in the Software without restriction, including
 ** without limitation the rights to use, copy, modify, merge, publish,
 ** distribute, sublicense, and/or sell copies of the Software, and to
 ** permit persons to whom the Software is furnished to do so, subject to
 ** the following conditions:
 ** 
 ** The above copyright notice and this permission notice shall be
 ** included in all copies or substantial portions of the Software.
 ** 
 ** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 ** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 ** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 ** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 ** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 ** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 ** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ** 
 **/

#ifndef _LOGGER_H_
#define _LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>

#define LOG_DEBUG 0
#define LOG_INFO  1
#define LOG_WARN  2
#define LOG_ERROR 3

#define LOG_LEVEL_CHARS "DIWEF"
#define LOG_MAX_MSG_LEN 1024

struct _Logger {
    int level;
    char *datetime_format;
    FILE *fp;
};

typedef struct _Logger Logger;

Logger * Logger_create( void );
void Logger_free(Logger *l);
void log_add(Logger *l, int level, const char *msg);
void log_debug(Logger *l, const char *fmt, ...);
void log_info(Logger *l, const char *fmt, ...);
void log_warn(Logger *l, const char *fmt, ...);
void log_error(Logger *l, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

/** Demo calls
 ** 
 ** int main()
 ** {
 **     char *foo;
 **     int bar = 10;
 ** 
 **     Logger *l = Logger_create();
 **     l->level = LOG_DEBUG;
 ** 
 **     log_debug(l, "%d", __LINE__);
 **     
 **     log_info(l, "Hello world");
 ** 
 **     foo = "Information";
 **     log_warn(l, "Foo[%-30s] Bar[%d]", foo, bar); 
 ** 
 **     log_error(l, "Foo Bar");
 ** 
 **     Logger_free(l);
 ** }
 ** 
 **/

#endif
