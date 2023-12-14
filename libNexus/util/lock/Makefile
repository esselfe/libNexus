DST= lock.x

INCLUDEDIR= -I. -I../../include
CFLAGS= -D_GNU_SOURCE -Wall -std=c99 -pedantic
LDFLAGS= -lpthread

OBJCMD= gcc -c

default:
	$(OBJCMD) ascii.c	$(CFLAGS) -o ascii.o
	$(OBJCMD) crypt.c	$(CFLAGS) -o crypt.o
	$(OBJCMD) threading.c $(CFLAGS) -o threading.o
	$(OBJCMD) lockUtil.c 	$(CFLAGS) -o lockUtil.o
	gcc $(INCLUDEDIR) $(CFLAGS) \
	$(LDFLAGS) \
	lock.c ascii.o crypt.o threading.o lockUtil.o	\
	-o $(DST)
#	rm -rf test*
#	cp -r ../test test2
