CFLAGS= -Wall -D_GNU_SOURCE -std=c99
LDFLAGS= -lNexus -lGLU -lGL
INCLUDES= -I ../libNexus/include

default:
	gcc inventory.c -I. $(CFLAGS) $(LDFLAGS) $(INCLUDES) -o inventory.x
