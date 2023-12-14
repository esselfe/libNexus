SRC= ascii.c crypt.c lock.c threading.c util.c
DST= lock

CFLAGS= -D_GNU_SOURCE -Wall -Werror -std=c99 -pedantic -g
LDFLAGS= -lpthread

default:
	gcc $(SRC)			\
	$(CFLAGS)	\
	$(LDFLAGS)						\
	-o $(DST)
#	rm -rf test*
#	cp -r ../test test2