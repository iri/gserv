CURDIR := $(shell pwd)
BASEDIR := $(abspath $(CURDIR)/../..)

PROJECT ?= $(notdir $(BASEDIR))
PROJECT := $(strip $(PROJECT))


LDLIBS += -lm -lpthread -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
CFLAGS += -fPIC -I/usr/local/include/SDL2 -D_THREAD_SAFE

UNAME := $(shell uname)


all: gserv gstest

gserv: gserv.c time.c logger.c udplib.c udp.c
	gcc -lpthread -o gserv gserv.c time.c logger.c udplib.c udp.c $(CFLAGS) $(LDLIBS)

gstest: gstest.c gs_api.c
	gcc -o gstest gstest.c gs_api.c $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o ./gserv ./gstest

