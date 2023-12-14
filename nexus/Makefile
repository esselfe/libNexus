DST= nexus.x

INCLUDEDIR= -I../libNexus/include -I./include
CFLAGS= -D_GNU_SOURCE -O3 -Wall -std=c99 -pedantic
LDFLAGS= -lc -lm -lGLU -lGL -lglut -lsensors -lNexus

OBJCMD= gcc -c

default:
	$(OBJCMD)  args.c $(CFLAGS) $(INCLUDEDIR) -o args.o
#$(OBJCMD)  events.c $(CFLAGS) $(INCLUDEDIR) -o events.o
	$(OBJCMD)  nexus.c $(CFLAGS) $(INCLUDEDIR) -o nexus.o
	echo "*******************" >/dev/null
	gcc $(INCLUDEDIR) $(CFLAGS) \
		$(LDFLAGS) \
	args.o \
	nexus.o \
	-o$(DST)
	rm *.o
