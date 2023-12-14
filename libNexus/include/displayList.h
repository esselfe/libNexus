#ifndef DISPLAYLIST_H
#define DISPLAYLIST_H 1

#include "mesh.h"
//#include "objects.h"

#define NODENAMESIZE 64

extern int listSize; /* this way list are managed as blocks of objects */
extern int headList;


// tmps
extern int floorDisplayListNodeID;	// the list with grid-lines
extern int floorDisplayListNodeID2;	// the list without
extern int menuEntryDisplayListNodeID;
extern int sphereDisplayListNodeID;

typedef struct DisplayListNode {
	struct DisplayListNode *prev, *next;
	int listID;		/* Position in the list */
	sMesh *mesh;
	char *name;
	float x, y, z;
} sDisplayListNode;
sDisplayListNode *rootDisplayListNode, *lastDisplayListNode;

void initDisplayListTree (void);
int displayListNodeID_byName (char *name);
struct DisplayListNode *displayListNode_byID (int id);
int registerDisplayListNode (char *name);	// Returns registered ID, just call glNewList() with that ID.
											// drawing.c::render() will call those IDs.

void NLcallDisplayLists (void);

#endif /* DISPLAYLIST_H */
