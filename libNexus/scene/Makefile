
CFLAGS= -D_GNU_SOURCE -Wall -std=c99 -pedantic -fPIC
LDFLAGS= -lc -lm -lglut

OBJCMD= gcc -c -I../include

default:
	$(OBJCMD) $(CFLAGS) camera.c -o camera.o
	$(OBJCMD) $(CFLAGS) map.c -o map.o
	$(OBJCMD) $(CFLAGS) osdata.c -o osdata.o
	$(OBJCMD) $(CFLAGS) displayList.c -o displayList.o
	$(OBJCMD) $(CFLAGS) drawing.c -o drawing.o
