CURDIR := $(shell pwd)
BASEDIR := $(abspath $(CURDIR)/../..)

PROJECT ?= $(notdir $(BASEDIR))
PROJECT := $(strip $(PROJECT))


LDLIBS += -lm -lpthread
CFLAGS += -fPIC

UNAME := $(shell uname)


all: gserv gstest

gserv: gserv.c time.c logger.c udplib.c udp.c
	gcc `simple2d --libs` -lpthread -o gserv gserv.c time.c logger.c udplib.c udp.c $(CFLAGS) $(LDLIBS)

gstest: gstest.c gs_api.c
	gcc `simple2d --libs` -o gstest gstest.c gs_api.c $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o ./gserv ./gstest

