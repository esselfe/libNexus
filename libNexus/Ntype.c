#include <stdio.h>

#include "Ntype.h"

void typeSetColor (Color *c, float r, float g, float b, float a) {
	c->r = r;
	c->g = g;
	c->b = b;
	c->a = a;
}

void typeSetPosition (Position *p, float x, float y, float z) {
	p->x = x;
	p->y = y;
	p->z = z;
}
