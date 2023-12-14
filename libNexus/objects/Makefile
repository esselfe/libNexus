DST= objects.o

INCLUDEDIR= ../include

CFLAGS= -D_GNU_SOURCE -Wall -std=c99 -pedantic -fPIC
LDFLAGS= -lc -lm -lglut

OBJCMD= gcc -c

default:
	$(OBJCMD) -I$(INCLUDEDIR) $(CFLAGS) objects.c -o $(DST)
