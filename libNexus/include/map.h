#ifndef MAP_H
#define MAP_H 1

#include "objects.h"

typedef struct Map {
	struct ObjectList *objectList;
	char *name;
} sMap;

#endif /* MAP_H */
