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

#include "logger.h"

Logger * Logger_create( void )
{
    Logger *l = (Logger *)malloc(sizeof(Logger));
    if ( l == NULL )
        return NULL;

    l->datetime_format = (char *)"%Y-%m-%d %H:%M:%S";
    l->level = LOG_INFO;
    l->fp    = stdout;

    return l;
}

void Logger_free(Logger *l)
{
    if ( l != NULL ) {
        if ( fileno(l->fp) != STDOUT_FILENO )
            fclose(l->fp);
        free(l);
    }
}

void log_add(Logger *l, int level, const char *msg)
{
    if (level < l->level) return;

    time_t meow = time(NULL);
    char buf[64];

    strftime(buf, sizeof(buf), l->datetime_format, localtime(&meow));
    fprintf(l->fp, "[%d] %c, %s : %s\n",
            (int)getpid(),
            LOG_LEVEL_CHARS[level],
            buf,
            msg);
}

void log_debug(Logger *l, const char *fmt, ...)
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_DEBUG, msg);
    va_end(ap);
}

void log_info(Logger *l, const char *fmt, ...)
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_INFO, msg);
    va_end(ap);
}

void log_warn(Logger *l, const char *fmt, ...)
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_WARN, msg);
    va_end(ap);
}

void log_error(Logger *l, const char *fmt, ...)
{
    va_list ap;
    char msg[LOG_MAX_MSG_LEN];

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    log_add(l, LOG_ERROR, msg);
    va_end(ap);
}
