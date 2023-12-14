#ifndef OBJECTS_H
#define OBJECTS_H 1

#include "map.h"
#include "mesh.h"

typedef struct Object {	// Mostly used for hud and 3D rendering
	struct Object *prev, *next;
	int type;
	int ID;
	float x, y, z;
	float scaleX, scaleY, scaleZ;
	sMesh *mesh;
	char *name;
} sObject;

typedef struct ObjectList {
	struct Object *rootObject, *lastObject;
	struct Map *map;	// parent
} sObjectList;

typedef struct TextObject {
	char *text;
	float dimension[3];
	float position[3];
	float rotation[3];
	float textColor[4];
	float backgroundColor[4];
} sTextObject;

void initObjects (void);
sTextObject *addTextObject (char *text, float posx, float posy, float posz);

#endif /* OBJECTS_H */
