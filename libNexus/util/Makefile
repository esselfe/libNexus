
INCLUDES= -I../include
CFLAGS= -D_GNU_SOURCE -Wall -Werror -std=c99 -pedantic
LDFLAGS= 

OBJCMD= gcc -c

default:
	make -C lock
	echo "**************************">/dev/null
	make -C memfiller
	echo "**************************">/dev/null
	make -C tester
	echo "**************************">/dev/null
	$(OBJCMD) util.c $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o util.o
