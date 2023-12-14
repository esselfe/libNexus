#ifndef NTYPE_H
#define NTYPE_H 1

typedef struct sColor {
	float r, g, b, a;
} Color;

typedef struct sPosition {
	float x, y, z;
} Position;

typedef struct Scale {
	float x, y, z;
} sScale;

typedef unsigned char mbool;

void typeSetColor (Color *c, float r, float g, float b, float a);
void typeSetPosition (Position *p, float x, float y, float z);


/* *************************** */
/* The reset of this file are types specific to the files */
/* that are using them. Planned for a grep or a GUITextEditor search function */

/* hud.h :: struct MessageBox */
#define MB_TYPE_NONE		0
#define MB_TYPE_QUESTION	1
#define MB_TYPE_INPUT		2
#define MB_TYPE_INFO		3
#define MB_TYPE_WARNING		4
#define MB_TYPE_ERROR		5

/* menu.h :: struct Entry */
#define ENTRY_TYPE_DUMMY					0
#define ENTRY_TYPE_ACTION					1
#define ENTRY_TYPE_SUBMENU					2
#define ENTRY_TYPE_MESHID					3

/* hud.h :: struct sHudObject_EntryList */
/* HOEL = hudObject EntryList */
#define HOEL_TYPE_BROWSER_FILES		0x01
#define HOEL_TYPE_EXTERN_FUNCS		0x02
#define HOEL_TYPE_MAIN_MENU			0x03
#define HOEL_TYPE_MESH_LIST			0x04

/* selectID.h */
#define SID_TYPE_NONE	0
#define SID_TYPE_POINT 	1
#define SID_TYPE_PLANE 	2
#define SID_TYPE_MENU	3
#define SID_TYPE_MESH	4


/* object.h :: struct Object */
#define OBJECT_TYPE_DUMMY 		0
#define OBJECT_TYPE_PLANE 		1
#define OBJECT_TYPE_CUBE 		2
#define OBJECT_TYPE_METER 		3
#define OBJECT_TYPE_TEXT		4
#define OBJECT_TYPE_MESH		5
#define OBJECT_TYPE_MENUENTRY	6

#endif	/* NTYPE_H */

