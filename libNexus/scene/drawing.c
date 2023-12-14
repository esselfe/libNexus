#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include <GL/glx.h>
#include <GL/glut.h>

#include "libNexus.h"
#include "browser.h"
#include "camera.h"
#include "commands.h"
#include "config.h"
#include "osdata.h"
#include "displayList.h"
#include "drawing.h"
#include "editor.h"
#include "events.h"
#include "hud.h"
#include "menu.h"
#include "mesh.h"
#include "meshEditing.h"
#include "memoryViewer.h"
#include "messages.h"
#include "race.h"
#include "state.h"

#include <X11/Xlib.h>
Display *display;
GC gc;

int rendering = 1;
int frame = 0;
short fps = 0;
mbool needRedraw = 1;
mbool lowRedraw = 0;
int codeParse_RootDisplayListNode_ID;
struct codeParse_RootDisplayListNode;
float codeDelta = 0.0;
float delta = 0.0;
float deltaStep = 0.01;
float tmpred1 = 0.0;
float tmpred2 = 0.0;
float ratio;
sMenu *drawnMenu;
int drawSphere = 1;

unsigned int frameSleepTime = 0;

void initDrawing (void) {
	lastWorld = NULL;
	dWorld = addWorld ("Nexus world");
	dWorld->scene = addScene (dWorld, "Nexus scene");
	
	drawnMesh = dWorld->scene->lastMesh;
	addPoint (drawnMesh->plane,0.0, 0.5, 0.0);
	addPoint (drawnMesh->plane, 1.0, 0.5, 0.0);
	addPoint (drawnMesh->plane, 1.0, 0.0, 0.0);
	sMesh *tmpmesh = loadMesh ("ArmyTruck");
	//sMesh *tmpmesh = loadMesh ("DefaultMesh");
	if (tmpmesh != NULL) drawnMesh = tmpmesh;
	
	renderingModeFunc = renderMainmenu;
	func.func = render2;
	func.ab = 16;
	
// 	glMatrixMode (GL_MODELVIEW);
// 	glPushMatrix ();
// 	glLoadIdentity ();
// 	gluLookAt (cam->x, cam->y, cam->z,
// 			   cam->lx, cam->ly, cam->lz,
// 	  0.0, 1.0, 0.0);
	
	//display = glXGetCurrentDisplay (); //XOpenDisplay (NULL);
	//if (display == NULL) printf ("initDrawing(): cannot open display\n");
	//gc = XDefaultGC (display, 0);
	//glXWaitGL ();
	//glXWaitX ();
}

 void idleFunc (void) {
	 glutPostRedisplay ();
 }
 
 void turnAlpha (mbool On_Off) {
	options.useAlpha = On_Off;
	if (options.useAlpha) {
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else glDisable (GL_BLEND);
 }

void reshape (int width, int height) {
	if (width == 0) width = 1;
	if (height == 0) height = 1;
	ratio = (float)width/(float)height;

	winWidth = width;
	winHeight = height;
	
	glViewport (0, 0, width, height);
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(45,ratio,0.01,1000);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glOrtho (0.0, winWidth, 0.0, winHeight, 0.1, 100.0);
	gluLookAt (cam->x, cam->y, cam->z,
			   cam->lx + cam->x, cam->ly + cam->y, cam->lz + cam->z,
	  0.0, 1.0, 0.0);

	if (mainMenuList.rootMenu != NULL) mainMenuList.rootMenu->y = winHeight - 16.0;
}

void render2 (void) {
	glPushMatrix ();
	
	glTranslatef (-10.0, 0.0, -10.0);
	
	int spherecnt = 20, cnt = 0;
	glPushMatrix ();
	glColor4f (0.0, 0.4, 0.1, 0.85);
	while (1) {
		glTranslatef (1.0, 0.0, 0.0);
		//glPushMatrix ();
		//glRotatef (delta, 0,0,1);
		glutWireSphere (1.0, 32, 32);
		glutSolidSphere (0.9, 32, 32);
		//glPopMatrix ();
		++cnt;
		if (cnt >= spherecnt) break;
	}
	glPopMatrix ();
	
	
	
	glPopMatrix ();
}

static float debugCountPointX = 0.0;
static float debugCountPointSize = 1.0;

void PPdebugCountX (void) {
	debugCountPointX += debugCountPointSize;
	if (debugCountPointX >= debugCountPointSize*5.0) debugCountPointX = 0.0;
}

//float tx=0.0, ty=0.5, tz=0.0;
//mbool msgOnce = 0;
//int doOnceAtInit = 1;
int lastSecond = 0;
void render (void) {
	++frame;
	if (program_ticks >= lastSecond + 1000) {
		//sprintf (fpsText, "%d FPS", frame);
		fps = frame;
//		hudUpdateFPS ();
//		hudUpdateClock ();
		lastSecond = program_ticks;
		frame = 0;
	}
	
	//if (lowRedraw) { 
	//	if (!msgOnce) {
			//addMsg (0,"Low redrawing");
			//printf ("lowRedraw,,,."); fflush (stdout); 
	//		msgOnce = 1;
	//	}
		//printf ("="); fflush (stdout); 
	//	usleep (500000);
	//}
	//else if (msgOnce) msgOnce = 0;
	
/*	if (!needRedraw) { 
		if (debug.drawing)
			printf ("?"); fflush (stdout);
		//usleep (250000);
		//usleep (125000);
		//usleep (31250);
//		usleep (75000);
		return; 
	}
	else needRedraw = 0;
*/	
	
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	glLoadIdentity ();
	gluLookAt (cam->x, cam->y, cam->z,
				cam->lx, cam->ly, cam->lz,
				0.0, 1.0, 0.0);
	
	//tz -= 0.1;
	
	// camera 'aiming' red dot
	/*glBegin (GL_POINTS);
	glColor4f (0.7,0.0,0.0, 1.0);
	glVertex3f (cam->lx, cam->ly, cam->lz);
	glEnd ();*/
	
	renderingModeFunc ();
	//func.func();
	
	NLcallDisplayLists ();
	
	if (rendering) {	/* 'rendering' is set in the picking funcs from events.c */
		setOrthographicProjection ();
		// Draw debugCounter
		//PPdebugCountX ();
		glPushMatrix ();
		 glTranslatef (0.0,0.0,0.0);
		 //glPointSize (2.0);
		 glBegin (GL_POINTS);
		  glColor4f (0.0,0.75,0.0,1.0);
		  //glVertex2f (1.0, 1.0);
		  //glVertex2f (debugCountPointX, winHeight);
		  //glVertex2f (frame, winHeight);
		  glVertex2f ((float)frame+4.0, 25.0);
		 glEnd ();
		glPopMatrix ();
		
		NLdrawHud ();
		setPerspectiveProjection ();
	}
	
	glPopMatrix ();
	//if (debug.drawing) {
	//	printf ("@"); fflush (stdout); }
	glutSwapBuffers ();
	
	/*if (doOnceAtInit) {
		printf ("init elapsed: %d\n", glutGet (GLUT_ELAPSED_TIME));
		doOnceAtInit = 0;
	}
	usleep (frameSleepTime);*/
	//usleep(250000);
	//needRedraw = 0;
}

void renderBrowser (void) {
	FileTree *ft = rootFileTree;
	FileSheet *fs = ft->rootfsheet;
	glPushMatrix ();
	glTranslatef (ft->x, ft->y, ft->z);
	glColor4f (0.15,0.15,0.15,0.85);
	glBegin (GL_POLYGON);
	 glVertex3f (0.0, 0.0, 0.0);
	 glVertex3f (0.0, 0.2 ,0.0);
	 glVertex3f (2.0, 0.2, 0.0);
	 glVertex3f (2.0, 0.0, 0.0);
	glEnd ();
	glColor4f (0.5, 0.5, 0.5, 0.75);
	hud3dText (ft->name, 0.1, 0.125, 0.075);
	glPopMatrix ();

	glPushMatrix ();
	glTranslatef (ft->x, ft->y, ft->z);
	while (1) {
		glTranslatef (fsheetSizeX + 0.2, 0.0, 0.0);
//		glPushMatrix ();
//		glTranslatef (fs->x, fs->y, fs->z);
		glColor4f (0.2, 0.35, 0.65, 0.5);
		glBegin (GL_POLYGON);
		 glVertex3f (0.0, 0.0, 0.0);
		 glVertex3f (0.0, fsheetSizeY, 0.0);
		 glVertex3f (fsheetSizeX, fsheetSizeY, 0.0);
		 glVertex3f (fsheetSizeX, 0.0, 0.0);
		glEnd ();
		
		glColor4f (0.5,0.5,0.5,0.75);
		hud3dText (fs->name, 0.1, 0.125, 0.075);
//		glPopMatrix ();
		if (fs->next == NULL) break;
		fs = fs->next;
	}
	glPopMatrix ();
}

void drawBuffer (void) {
	glPushMatrix ();
/*	
	glTranslatef (0.0, 0.5, -1.0);
	glColor4f (0.2, 0.5, 0.3,  0.75);
	glutWireSphere (1.0, 32, 32);
	
*/	glPopMatrix ();
}

void drawPrompt (void) {
	// Prompt buffer area background
	glPushMatrix ();
	glTranslatef (drawnPrompt->x+ENTRY_SIZEX, drawnPrompt->y-20.0, 0.9);

	glColor4f (0.2, 0.2, 0.2, 0.85);
	glBegin (GL_POLYGON);
	 glVertex3f (0.0,		0.0,	0.0);
	 glVertex3f (0.0,		drawnPrompt->sizeY, 0.0);
	 glVertex3f (drawnPrompt->sizeX, drawnPrompt->sizeY, 0.0);
	 glVertex3f (drawnPrompt->sizeX, 0.0, 0.0);
	glEnd ();


	drawingPrompt = 1;
	glColor4fv (hudTextColor);
	hud2dText (2.0, 14.0, drawnPrompt->buffer);
	drawingPrompt = 0;
	
	glPopMatrix ();

	// Prompt text area background and text
	glPushMatrix ();
	glTranslatef (drawnPrompt->x+ENTRY_SIZEX, drawnPrompt->y - drawnPrompt->sizeY*2, 0.9);
	//glTranslatef (0.0, -20.0, 0.0);

	glColor4f (0.15, 0.15, 0.15, 0.85);
	glBegin (GL_POLYGON);
	 glVertex3f (0.0, 0.0, 0.0);
	 glVertex3f (0.0, drawnPrompt->sizeY, 0.0);
	 glVertex3f (drawnPrompt->sizeX, drawnPrompt->sizeY, 0.0);
	 glVertex3f (drawnPrompt->sizeX, 0.0, 0.0);
	glEnd ();


	drawingPrompt = 1;
	glColor4fv (hudTextColor);
	hud2dText (2.0, 14.0, drawnPrompt->text);
	drawingPrompt = 0;

	glPopMatrix ();
	
	drawnPrompt->cursorPosition = 2.0;
}


/* renderDisplayListNode_RootID is set in initDisplatListTree() */
void renderCodeParse (void) {
/*	glPushMatrix ();
	glTranslatef (0.0, 0.0, 0.0);
	glCallList (renderDisplayListNode_RootID);
*/
	/*	glColor4f (0.2, 0.6, 0.2, 0.4);
	glutSolidCube (0.9);
	glColor4f (0.4, 0.7, 0.4, 0.4);
	glutWireCube (1.0);
*/

//	glPopMatrix ();
}

void renderEditor (void) {
	//glPushMatrix ();
	//glTranslatef (0.0, 0.0, 0.0);
	//moveSelection ();
	drawMesh (drawnMesh);
	
	if (movingSelectionUD != 0 || movingSelectionLR != 0 || movingSelectionIO != 0) {
		moveSelection ();
		needRedraw = 1;
	}
	
	//glColor4f (0.7, 0.8, 0.7, 0.5);
	//glRasterPos3f (drawnMesh->plane->point->x, 
	//			   drawnMesh->plane->point->y, 
	 //  drawnMesh->plane->point->z);
	//glBitmap(16, 16, 0.0, 0.0, 0.0, 0.0, fire);
	//glPopMatrix ();
}

void renderGame (void) {	
	glPushMatrix ();
	glTranslatef (0.0, 0.0, 0.0);
	glRotatef (0.25, 1,1,0);
	glColor3f (0.2, 0.3, 0.2);
	glutSolidCube (0.9);
	glColor3f (0.3, 0.4, 0.3);
	glutWireCube (1.0);
	glPushMatrix ();
}

void renderMainmenu (void) {
	return;
	// There's a displayListNode replacing this code
	//glPushMatrix ();
	/*glTranslatef (0.0, 0.0, 0.0);
	glColor4f (0.1,0.2,0.1,0.45);
	glutSolidSphere (0.9, 32, 32);
	 glColor4f (tmpred1, 0.6, 0.0, 0.7);
	  glutWireSphere (1.0, 32, 32);*/
	//glPopMatrix ();
}

void renderMem (void) {
	glPushMatrix ();
	glTranslatef (0.0, 0.0, 0.0);
	drawVerticalMeter (&ramMeter);
	if (options.showUsb) drawVerticalMeter (&usbMeter);
	//drawVerticalMeter (&loadMeter);
	drawVerticalMeter (&swapMeter);
	//drawVerticalMeter (&highmemMeter);
	glPopMatrix ();
}

void renderRace (void) {
	firstCar.posz -= firstCar.speed;
	cam->z -= firstCar.speed;
	cam->lz -= firstCar.speed;
	glPushMatrix ();
	glTranslatef (firstCar.posx, 0.0, firstCar.posz);
	//glRotatef (cam->angle, 0.0,1.0,0.0);
	glColor4f (0.2, 0.3, 0.2, 0.85);
	glBegin (GL_POLYGON);
	 glVertex3f (-1.5, 0.3, -2.5);
	 glVertex3f (-1.5, 1.0, -2.5);
	 glVertex3f (1.5, 1.0, -2.5);
	 glVertex3f (1.5, 0.3, -2.5);
	glEnd ();
	glPopMatrix ();
}

/* for 2D-mode navigation */
void drawBrowser (void) {
	//struct FileSheet *fsheet = rootFileTree->rootfsheet;	
	glPushMatrix ();
	glTranslatef (10.0, 0.0, -0.1);
	glColor4f (0.2, 0.3, 0.2, 0.75);
	glBegin (GL_POLYGON);
	glVertex3f (0.0, 0.0,  0.0);
	glVertex3f (0.0, 2.0, 0.0);
	glVertex3f (2.0, 2.0, 0.0);
	glVertex3f (2.0, 0.0, 0.0);
	glEnd ();
	glPopMatrix ();
}
/*
void drawCpuMeter (struct CpuMeter *meter) {
	glPushMatrix ();
	glBegin (GL_POLYGON);
	glColor4fv (meter->color);
	glVertex3f (meter->x-0.1, meter->y-0.1, 0.0);
	glVertex3f (meter->x+meter->width+0.1, meter->y-0.1, 0.0);
	glVertex3f (meter->x+meter->width+0.1, meter->y+meter->height+0.1, 0.0);
	glVertex3f (meter->x-0.1, meter->y+meter->height+0.1, 0.0);
	glEnd ();
	glPopMatrix ();
}*/

int once16 = 0;
void drawMesh (sMesh *mesh) {
	glInitNames ();
	glPushName (0);
	
	glPushMatrix ();
	glTranslatef (mesh->x, mesh->y, mesh->z);
	Plane *curPlane = mesh->plane;
	Point *curPoint = curPlane->point;
	while (1) {	// All planes
		if (debug.showPoint) while (1) {	// All points
			glPushMatrix ();
			glTranslatef (curPoint->x, curPoint->y, curPoint->z);
			
			glColor4fv (curPoint->color);
			glLoadName (curPoint->ID);
			glutSolidCube (0.025);
			if (curPoint->hovered && rendering) {
				glBegin (GL_POLYGON);
				glColor4f (0.1,0.3,0.15, 0.5);
				glVertex3f (0.0, 0.0, 0.01);
				glVertex3f (0.0, 0.1, 0.01);
				glVertex3f (0.1, 0.0, 0.01);
				glEnd ();
			}
			if (debug.showids) {
				sprintf (msgbuf, "%d", curPoint->ID);
				hudTextSetOnceColor (0.75,0.75,0.75,0.75);
				hud3dText (msgbuf, 0.0, 0.0, 0.075);
			}
			glPopMatrix ();
			if (curPoint->next) curPoint = curPoint->next;
			else break;
		}
		
		// Draw contour
		if (debug.showMeshLines) {
			curPoint = curPlane->point;
			glPushMatrix ();
			glTranslatef (mesh->x, mesh->y, mesh->z);
			glColor4fv (lineDefaultColor);
			glBegin (GL_LINE_STRIP);
			while (1) {
				glVertex3f (curPoint->x, curPoint->y, curPoint->z);
				if (curPoint->next == NULL) { // The only link to the first point
					curPoint = curPlane->point;
					glVertex3f (curPoint->x, curPoint->y, curPoint->z);
					break;
				}
				else curPoint = curPoint->next;
			}
			glEnd ();
			glPopMatrix ();
		}
		
		// Draw surface filling
		curPoint = curPlane->point;
		glPushMatrix ();
		glTranslatef (mesh->x, mesh->y, mesh->z);
		glColor4fv (curPlane->color);
		glLoadName (curPlane->ID);
		glBegin (GL_POLYGON);
		while (1) {
			glVertex3f (curPoint->x, curPoint->y, curPoint->z);
			if (curPoint->next == NULL) {
				curPoint = curPlane->point;
				glVertex3f (curPoint->x, curPoint->y, curPoint->z);
				break;
			}
			else curPoint = curPoint->next;
		}
		glEnd ();
		glPopMatrix ();
		
		if (curPlane->next == NULL) break;
		else {
			curPlane = curPlane->next;
			curPoint = curPlane->point;
		}
	}
	glPopMatrix ();
}

void drawTextObject (sTextObject *obj) {
	glColor4fv (obj->textColor);
	hud3dText (obj->text, 
			   obj->position[0], obj->position[1], obj->position[2]);
}

void drawVerticalMeter (VerticalMeter *meter) {
	//glCullFace (GL_FRONT);
	glPushMatrix ();
	 glColor4fv (meter->trimColor);
	 glTranslatef (meter->x, meter->height/2, meter->z);
	 glScalef (1.0, meter->height, 1.0);
	 glutWireCube (1.0);
	glPopMatrix ();
	
	glPushMatrix ();
	 glColor4fv (meter->color);
	 glTranslatef (meter->x, (float)meter->value/200000000, meter->z);
	 glScalef (1.0, (float)meter->value/100000000, 1.0);
	 glutSolidCube (0.99);
	 glColor4fv (meter->trimColor);
	 glutWireCube (1.0);
	glPopMatrix ();
	
	if (mode == MODE_MEM && cam->x < meter->x + 7.0) {
		drawTextObject (meter->label);
		drawTextObject (meter->svalue);
		drawTextObject (meter->svalueDiff);
	}
	/*
	// Trim current value
	glPushMatrix ();
	glTranslatef (meter->x, meter->value, meter->z);
	glColor4f (0.8, 0.8, 0.8, 0.8);
	glBegin (GL_LINE_STRIP);
	 glVertex3f (-meter->width/2-0.01, 0.0, meter->width/2+0.01);
	 glVertex3f (meter->width/2+0.01, 0.0, meter->width/2+0.01);
	 glVertex3f (meter->width/2+0.01, 0.0, -meter->width/2-0.01);
	 glVertex3f (-meter->width/2-0.01, 0.0, -meter->width/2-0.01);
	 glVertex3f (-meter->width/2-0.01, 0.0, meter->width/2+0.01);
	glEnd ();
	glPopMatrix ();
	
	// Trim min value
	glPushMatrix ();
	glTranslatef (meter->x, meter->valueMin-0.05, meter->z);
	glColor4f (0.4, 0.4, 0.8, 0.8);
	glBegin (GL_LINE_STRIP);
	glVertex3f (-meter->width/2-0.01, 0.0, meter->width/2+0.02);
	glVertex3f (meter->width/2+0.01, 0.0, meter->width/2+0.02);
	glVertex3f (meter->width/2+0.01, 0.0, -meter->width/2-0.02);
	glVertex3f (-meter->width/2-0.01, 0.0, -meter->width/2-0.02);
	glVertex3f (-meter->width/2-0.01, 0.0, meter->width/2+0.02);
	glEnd ();
	glPopMatrix ();
	
	// Trim max value
	glPushMatrix ();
	glTranslatef (meter->x, meter->valueMax+0.05, meter->z);
	glColor4f (0.8, 0.4, 0.4, 0.8);
	glBegin (GL_LINE_STRIP);
	glVertex3f (-meter->width/2-0.01, 0.0, meter->width/2+0.03);
	glVertex3f (meter->width/2+0.01, 0.0, meter->width/2+0.03);
	glVertex3f (meter->width/2+0.01, 0.0, -meter->width/2-0.03);
	glVertex3f (-meter->width/2-0.01, 0.0, -meter->width/2-0.03);
	glVertex3f (-meter->width/2-0.01, 0.0, meter->width/2+0.03);
	glEnd ();
	glPopMatrix ();
	*/
	 //glMatrixMode (GL_MODELVIEW);
	/*glPushMatrix ();
	glColor4f (tmpred1, 0.6, 0.0, 0.7);
	glTranslatef (0.0, 0.0, 0.0);
	glRotatef (delta, 0.0, 1.0, 0.0);
	glutWireSphere (1.0, 32, 32);
	glColor4f (0.1,0.2,0.1,0.5);
	glutSolidSphere (0.99, 32, 32);
	glPopMatrix ();
	*/
	//glPushMatrix ();
	//glColor4fv (hudTextColor);
	// if (mode == MODE_MEM) {
	//	 hud3dText (meter->label->text, 
	//				meter->label->position[0], meter->label->position[1], meter->label->position[2]);
	//	 hud3dText (meter->svalue->text, 
	//				meter->svalue->position[0], meter->svalue->position[1], meter->svalue->position[2]);
	//	 hud3dText (meter->svalueDiff->text, 
	//				meter->svalueDiff->position[0], meter->svalueDiff->position[1], meter->svalueDiff->position[2]);
	//	hud3dText (meter->svalueMax, meter->x-meter->width/2, meter->y*2+1.5, meter->width/2);
	//	hud3dText (meter->svalue, 	meter->x-meter->width/2, meter->y*2+1.0, meter->width/2);
	//	hud3dText (meter->svalueMin, meter->x-meter->width/2, meter->y*2+0.5, meter->width/2);
	//}
	//glPopMatrix ();
}

void drawSelectBox (void) {
	printf ("."); fflush (NULL);
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix ();
	glTranslatef (firstMouseX, firstMouseY, 0.9);
	glBegin (GL_POLYGON);
	glColor4f (0.25, 0.3, 0.4, 0.3);
		glVertex2f (0.0, 0.0);
		glVertex2f ((double)lastMouseX*1.0-firstMouseX, 0.0);
		glVertex2f ((double)lastMouseX*1.0-firstMouseX, (double)lastMouseY*1.0-firstMouseY);
		glVertex2f (0.0, (double)lastMouseY*1.0-firstMouseY);
	glEnd ();
	
	// Draw contour
	glLineWidth (2.0);
	glBegin (GL_LINE_STRIP);
	glColor4f (0.45, 0.5, 0.6, 0.5);
	glVertex2f (0.0, 0.0);
	glVertex2f ((float)lastMouseX*1.0-firstMouseX, 0.0);
	glVertex2f ((float)lastMouseX*1.0-firstMouseX, (float)lastMouseY*1.0-firstMouseY);
	glVertex2f (0.0, (float)lastMouseY*1.0-firstMouseY);
	glVertex2f (0.0, 0.0);
	glEnd ();
	glLineWidth (1.0);
	glPopMatrix ();
	//glDisable(GL_BLEND);
}

