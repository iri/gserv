CURDIR := $(shell pwd)
BASEDIR := $(abspath $(CURDIR)/../..)

PROJECT ?= $(notdir $(BASEDIR))
PROJECT := $(strip $(PROJECT))


UNAME_SYS := $(shell uname -s)
ifeq ($(UNAME_SYS), Darwin)
	CFLAGS += -fPIC -I/usr/local/include/SDL2 -D_THREAD_SAFE
else ifeq ($(UNAME_SYS), Linux)
	CFLAGS += -fPIC -I/usr/include/SDL2 -D_THREAD_SAFE
endif

LDLIBS += -lm -lpthread  -lSDL2

all: gserv gstest

gserv: gserv.c time.c logger.c udplib.c udp.c
	gcc -lpthread -o gserv gserv.c time.c logger.c udplib.c udp.c $(CFLAGS) $(LDLIBS)

gstest: gstest.c gs_api.c
	gcc -o gstest gstest.c gs_api.c $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o ./gserv ./gstest

