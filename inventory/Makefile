CFLAGS= -Wall -Werror -D_GNU_SOURCE -std=c99
LDFLAGS= -lNexus
INCLUDES= -I ../libNexus/include

default:
	gcc inventory.c -I. $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o inventory.x
