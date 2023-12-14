#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libNexus.h"
#include "hud.h"
#include "displayList.h"
#include "objects.h"
#include "osdata.h"
/*
TextObject {
	char *text;
	float dimension[3];
	float position[3];
	float rotation[3];
	float scale[3];
	float textColor[4];
	float backgroundColor[4];
};*/
void initObjects (void) {
	
	
	
	
	return;
}

sTextObject *addTextObject (char *text, float posx, float posy, float posz) {
	sTextObject *obj = malloc (sizeof (sTextObject));
	obj->text = malloc (strlen(text)+128);
	sprintf (obj->text, "%s", text);
	obj->dimension[0] = 1.0;
	obj->dimension[1] = 1.0;
	obj->dimension[2] = 1.0;
	obj->position[0] = posx;
	obj->position[1] = posy;
	obj->position[2] = posz;
	obj->rotation[0] = 0.0;
	obj->rotation[1] = 0.0;
	obj->rotation[2] = 0.0;
	obj->textColor[0] = hudTextColor[0];
	obj->textColor[1] = hudTextColor[1];
	obj->textColor[2] = hudTextColor[2];
	obj->textColor[3] = hudTextColor[3];
	obj->backgroundColor[0] = 0.5;
	obj->backgroundColor[1] = 0.5;
	obj->backgroundColor[2] = 0.5;
	obj->backgroundColor[3] = 0.5;
	return obj;
}
