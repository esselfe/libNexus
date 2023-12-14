
CFLAGS= -D_GNU_SOURCE -Wall -std=c99 -pedantic -fPIC

OBJCMD= gcc -g -c -I../include

default:
	$(OBJCMD) $(CFLAGS) browser.c -o browser.o
	$(OBJCMD) $(CFLAGS) buffer.c -o buffer.o
	$(OBJCMD) $(CFLAGS) editor.c -o editor.o
	$(OBJCMD) $(CFLAGS) memoryViewer.c -o memoryViewer.o
	$(OBJCMD) $(CFLAGS) race.c -o race.o
