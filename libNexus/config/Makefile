DST= config.o

CFLAGS= -D_GNU_SOURCE -Wall -Werror -std=c99 -pedantic
LDFLAGS= 

OBJCMD= gcc -c -I../include

default:
	$(OBJCMD) config.c $(CFLAGS) $(LDFLAGS) -o $(DST)
