DST= config.o

CFLAGS= -D_GNU_SOURCE -Wall -std=c99 -pedantic -fPIC
LDFLAGS= 

OBJCMD= gcc -c -I../include

default:
	$(OBJCMD) config.c $(CFLAGS) $(LDFLAGS) -o $(DST)
