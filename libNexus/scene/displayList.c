#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <GL/glut.h>

#include "camera.h"
#include "config.h"
#include "displayList.h"
#include "drawing.h"
#include "hud.h"
#include "menu.h"
#include "messages.h"
//#include "libNexus.h"
#include "objects.h"
#include "state.h"

int listSize = 25;
int headList;
int floorDisplayListNodeID;
int floorDisplayListNodeID2;
int menuEntryDisplayListNodeID;
int sphereDisplayListNodeID;
sDisplayListNode *rootDisplayListNode, *lastDisplayListNode;

void initDisplayListTree (void) {
	if (debug.displayList) printf ("initDisplayList(): started\n");
	rootDisplayListNode = NULL;
	headList = glGenLists (listSize);
	int tmpID;

	// List menu entry
	tmpID = registerDisplayListNode ("menuEntry");
	glNewList (tmpID, GL_COMPILE);
	menuEntryDisplayListNodeID = tmpID;
	if (debug.displayList) printf ("\tmenuEntry list ID %d\n", tmpID);
	glPushMatrix ();
	//setOrthographicProjection ();
//	drawMenu ();
	glColor4f (0.25,0.25,0.25,0.85);
	glBegin (GL_TRIANGLES);
	glVertex3f (0.0,0.0,0.0);
	glVertex3f (0.0,10.0,0.0);
	glVertex3f (20.0,0.0,0.0);
	glEnd();
	//setPerspectiveProjection ();
	glPopMatrix ();
	glEndList ();
	
	/* List a floor */
	static short cnt;
	float tmpx = -50.0, tmpz = -50.0;
	// Set a floor
	tmpID = registerDisplayListNode ("Floors");
	if (debug.displayList) printf ("\tFloor list ID %d\n", tmpID);
	floorDisplayListNodeID = tmpID;
	
	glNewList (tmpID, GL_COMPILE);
	glPushMatrix ();
	glTranslatef (0.0, 0.0, 0.0);

	
	// Center mark
/*	glColor4f (0.5,0.0,0.0,0.3);
	glutSolidSphere (0.009,16,16);
	glColor4f (0.75,0.0,0.0,0.3);
	glutWireSphere (0.01,16,16);
*/	
	glColor4f (0.0,0.3,0.0,1.0);
	tmpx = -50.0;
	tmpz = -50.0;
	for (cnt = 0;  cnt < 100;  cnt++) {
		if (cnt == 0 || cnt % 10 == 0)	glColor4f (0.55,0.3,0.0,1.0); 
		else glColor4f (0.0,0.3,0.0,1.0); 
		glBegin (GL_LINES);
		glVertex3f (tmpx, 0.0, tmpz);
		glVertex3f (-tmpx, 0.0, tmpz);
		tmpz += 1.0;
		glEnd ();
		//if (cnt == 0 || cnt % 10 == 0)	glColor4f (0.0,0.3,0.0,1.0); 
	}
	glBegin (GL_LINES);
	tmpx = -50.0, tmpz = -50.0;
	for (cnt = 0;  cnt < 100;  cnt++) {
		if (cnt % 10 == 0)	glColor4f (0.7,0.3,0.0,1.0);
		else glColor4f (0.0,0.3,0.0,1.0);
		glVertex3f (tmpx, 0.0, tmpz);
		glVertex3f (tmpx, 0.0, -tmpz);
		tmpx += 1.0;
		//if (cnt % 10 == 0) glColor4f (0.0,0.3,0.0,1.0); 
	}
	glEnd ();
	
	if (cam->y < 0.0) glFrontFace (GL_CCW);
	glColor4f (0.1,0.1,0.1,1.0);
	glBegin (GL_POLYGON);
	glVertex3f (-50.0, -0.05, -50.0);
	glVertex3f (50.0, -0.05, -50.0);
	glVertex3f (50.0, -0.05, 50.0);
	glVertex3f (-50.0, -0.05, 50.0);
	glEnd ();
	if (cam->y < 0.0) glFrontFace (GL_CW);
	glPopMatrix ();
	glEndList ();
	
	// gridLine-less one
	tmpID = registerDisplayListNode ("Floors2");
	floorDisplayListNodeID2 = tmpID;
	if (debug.displayList) printf ("\tFloors2 list ID %d\n", tmpID);
glNewList (tmpID, GL_COMPILE);
	glPushMatrix ();
	glTranslatef (0.0, 0.0, 0.0); 
	glFrontFace (GL_CW);
		glColor4f (0.1,0.1,0.1,1.0);
		glBegin (GL_POLYGON);
		glVertex3f (-50.0, -0.05, -50.0);
		glVertex3f (50.0, -0.05, -50.0);
		glVertex3f (50.0, -0.05, 50.0);
		glVertex3f (-50.0, -0.05, 50.0);
		glEnd ();
	glPopMatrix ();
glEndList ();


	// for a game, should be a mesh defining minerals
	tmpID = registerDisplayListNode ("Minerals");
	glNewList (tmpID, GL_COMPILE);
	if (debug.displayList) printf ("\tMinerals list ID %d\n", tmpID);
	glPushMatrix ();
	glTranslatef (0.0, 0.0, -5.0);
	 glBegin (GL_TRIANGLE_STRIP);
	 glColor4f (0.0, 0.5, 0.4, 0.75);
	  glVertex3f (0.0, 0.0, 0.0);
	  glVertex3f (0.0, 1.0, 0.0);
	  glVertex3f (0.5, 1.2, 0.0);
//	  glVertex3f (1.0, 1.0, 0.0);
//	  glVertex3f (1.25, 0.0, 0.0);
	 glEnd ();
	glPopMatrix ();
	glEndList ();

	/* List a sphere */
	tmpID = registerDisplayListNode ("Sphere3232");
	sphereDisplayListNodeID = tmpID;
	if (debug.displayList) printf ("\tmainMenu sphere list ID %d\n", tmpID);
	glNewList (tmpID, GL_COMPILE);
	
	//glPushMatrix ();
	//glTranslatef (0.0, 0.0, 0.0);
	 glColor4f (0.0, 0.5, 0.0, 0.7);
	 glutWireSphere (1.0, 32, 32);

	 glColor4f (0.1, 0.2, 0.1, 0.45);
	 glutSolidSphere (0.9, 32, 32);

	//glPopMatrix ();

	glEndList ();


	/* set the codeParse mode stuff */
	tmpID = registerDisplayListNode ("codeParse1616");
	codeParse_RootDisplayListNode = displayListNode_byID (tmpID);
	codeParse_RootDisplayListNode_ID = tmpID;
	if (debug.displayList) printf ("\tcodeParse list ID %d\n", tmpID);
	struct DisplayListNode *node = codeParse_RootDisplayListNode;
	node->x = 0.0;
	node->y = 0.0;
	node->z = 0.0;
	glNewList (tmpID, GL_COMPILE);
	
	glPushMatrix ();
	glTranslatef (0.0, 2.0, 0.0);
	glScalef (8.0, 4.0, 0.1);
	 glColor4f (0.2, 0.6, 0.2, 0.4);
	 glutSolidCube (1.0);

	
	glPopMatrix ();
	 glPushMatrix ();
	 glTranslatef (node->x, node->y, node->z+0.2);
	 glColor4fv (hudTextColor);
	 hud3dText ("codeParse", node->x, node->y, node->z);
	 glPopMatrix ();

	 glEndList ();
	 if (debug.displayList) printf ("initDisplayList(): done\n");
}

int displayListNodeID_byName (char *name) {
	struct DisplayListNode *node = rootDisplayListNode;
	while (1) {
		if (strcmp (node->name, name) == 0) {
			return node->listID;
		}
		if (node->next == NULL) break;
		else {
			node = node->next;
		}
	}
	printf ("displayListNodeID_byName(): found no ID for %s\n", name);
	return 0;
}

struct DisplayListNode *displayListNode_byID (int id) {
	struct DisplayListNode *node = rootDisplayListNode;
	while (1) {
		if (node->listID == id) return node;
		else {
			if (node->next == NULL) break;
			else node = node->next;
		}
	}
	printf ("displayListNode_byID(): found no list node for ID# %d\n", id);
	return NULL;
}

int registerDisplayListNode (char *name) {
	struct DisplayListNode *node;
	if (rootDisplayListNode == NULL) {
		rootDisplayListNode = malloc (sizeof (struct DisplayListNode));
		node = rootDisplayListNode;
		node->listID = 1;
		node->prev = NULL;
		node->next = NULL;
	}
	else {
		node = lastDisplayListNode;
		node->next = malloc (sizeof(struct DisplayListNode));
		if (node == NULL) {
			sprintf (msgbuf2, "addDisplayListNode (%s): malloc() node->next", name);
			perror (msgbuf2);
			return 0;
		}
		node->next->prev = node;
		node->next->next = NULL;
		node = node->next;
		node->listID = node->prev->listID + 1;
	}
	node->name = malloc (strlen (name)+1);
	if (node->name == NULL) {
		sprintf (msgbuf2, "addDisplayListNode (%s): malloc() node->name", name);
		perror (msgbuf2);
		return 0;
	}
	sprintf (node->name, "%s", name);
	
	lastDisplayListNode = node;
	return node->listID;
}

void NLcallDisplayLists (void) {
	if (mode == MODE_MAINMENU) {
		if (drawSphere) {
			glPushMatrix ();
			glRotated (delta, 0,-1,0);
			glCallList (sphereDisplayListNodeID);
			glPopMatrix ();
		}
	}
	if (options.showFloor) {
		if (options.showFloorLines) glCallList (floorDisplayListNodeID);
		else {
			glCallList (floorDisplayListNodeID2);
		}
	}
	/*sDisplayListNode *dnode = rootDisplayListNode;
	while (1) {
	if (strcmp ("Sphere3232", dnode->name) == 0) {
	if (mode == MODE_MAINMENU) {
	glPushMatrix ();
	glTranslatef (0.0, 0.0, 0.0);
	glRotated (delta, 0,-1,0);
	glCallList (dnode->listID);
	glPopMatrix ();
				
	glPushMatrix ();
	glTranslatef (1.0, 0.0, 0.0);
	glRotated (delta, 0,-1,0);
	glCallList (dnode->listID);
	glPopMatrix ();
				
	glPushMatrix ();
	glTranslatef (2.0, 0.0, 0.0);
	glRotated (delta, 0,-1,0);
	glCallList (dnode->listID);
	glPopMatrix ();
}
}
	else if (strcmp ("Floors", dnode->name) == 0) {
	if (options.showFloor) {
	glCallList (dnode->listID);
}
	else {
	glColor4f (0.5,0.0,0.0,0.3);
	glutSolidSphere (0.09,16,16);
	glColor4f (0.75,0.0,0.0,0.3);
	glutWireSphere (0.1,16,16);
}
}
	else if (strcmp ("codeParse1616", dnode->name) == 0) {
	if (mode == MODE_CODEPARSE) {
	glPushMatrix ();
	glTranslatef (codeParse_RootDisplayListNode->x, codeParse_RootDisplayListNode->y, codeParse_RootDisplayListNode->z);
	glCallList (codeParse_RootDisplayListNode_ID);
	glPopMatrix ();
}
}
	else if (strcmp ("Minerals", dnode->name) == 0) {
	if (mode == MODE_GAME) {
	glCallList (dnode->listID);
}
}
	else {
	if (once64) printf ("render(): newList ID #%d\n", dnode->listID);
	glCallList (dnode->listID);
}
	if (dnode->next == NULL) break;
	dnode = dnode->next;
}*/
}
