
INCLUDEDIR= ../include

CFLAGS= -D_GNU_SOURCE -Wall -fPIC
LDFLAGS= -lc -lm -lglut

OBJCMD= gcc -c -I../include

default:
	$(OBJCMD) $(CFLAGS) commands.c -o commands.o
	$(OBJCMD) $(CFLAGS) hud.c -o hud.o
	$(OBJCMD) $(CFLAGS) messages.c -o messages.o
	$(OBJCMD) $(CFLAGS) menu.c  -o menu.o
