DST= objects.o

INCLUDEDIR= ../include

CFLAGS= -D_GNU_SOURCE -Wall -Werror -std=c99 -pedantic
LDFLAGS= -lc -lm -lglut

OBJCMD= gcc -c

default:
	$(OBJCMD) -I$(INCLUDEDIR) $(CFLAGS) objects.c -o $(DST)
