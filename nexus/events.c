#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <GL/glut.h>

#include "browser.h"
#include "camera.h"
#include "commands.h"
#include "config.h"
#include "osdata.h"
#include "drawing.h"
#include "editor.h"
#include "events.h"
#include "hud.h"
#include "menu.h"
#include "mesh.h"
#include "meshEditing.h"
#include "messages.h"
#include "libNexus.h"
#include "Ninit.h"
#include "race.h"
#include "state.h"
#include "tester.h"
#include "threading.h"
#include "util.h"

Plane *plane = NULL, *curSelectedPlane = NULL, *lastcurSelectedPlane = NULL, *curHoveredPlane = NULL;
Point *point = NULL, *curSelectedPoint = NULL, *lastcurSelectedPoint = NULL, *curHoveredPoint = NULL;
int lastHoveredPlaneID = 0;
int lastHoveredPointID = 0;
int hoveredPointID = 0;
int hoveredPlaneID = 0;
struct Entry *menuLastSelectedEntry = NULL;
struct Entry *curHoveredEntry;
mbool selecting = 0;
mbool hoveringPlane = 0;
mbool hoveringPoint = 0;
mbool hoveringMenu = 0;
int lastClick = 0;
int firstMouseX = 0, firstMouseY = 0;
int lastMouseX = 0, lastMouseY = 0;
int cursorX = 0, cursorY = 0;
int mod = 0;
int cntx = 0; // used in the message test loop, the 't' key
int once64 = 0;

sMesh *evmesh;
struct MeshNameList *evlist;

GLuint selectBuf [SELECT_BUFFER_SIZE] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
	0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

void normalKeysDown (unsigned char key, int x, int y) {
//	printf ("key: %d :%c:\n", key, key);
	if (key == 27) {	// Escape
		menuHideAll ();
		//printf ("\n");
		//exitNexus (STATE_EVENT, STATE_OK);
	}
	if (drawnPrompt->drawn) {
		//glutSetKeyRepeat (GLUT_KEY_REPEAT_ON);
		parsePrompt (key);
		return;
	}
	if (key == 127) { // del
		if (mode == MODE_EDITOR) {
			deleteSelectedPoint ();
			return;
		}
	}
	else if (key == 48) { // numpad zero switch between camera and selection movements
		if (numpadMode == MODE_CAMERA) {
			addMsg (0, "Numpad now moves editor's selected mesh points");
			numpadMode = MODE_SELECTION;
		}
		else if (numpadMode == MODE_SELECTION) {
			addMsg (0, "Numpad now moves camera");
			numpadMode = MODE_CAMERA;
		}
		return;
	}
	mod = glutGetModifiers ();
//	printf ("mod: %d\n", mod);
	if (mod == GLUT_ACTIVE_SHIFT) { //printf ("shift + :%c: | %d\n",key,key);
		
	}
	if (mod == GLUT_ACTIVE_CTRL) { printf ("ctrl + :%c: | %d\n",key,key);
		switch (key) {
			case 3: //'c'
				//printf ("ctrl-c\n");
				break;
			case 18: //'r'
				//printf ("ctrl-r\n");
				break;
			case 19: //'s'
				//addMsg (0, "Saving mesh %s", drawnMesh-name);
				saveMesh (drawnMesh);
				break;
				break;
			default:
				break;
		}
	}
	else if (mod == GLUT_ACTIVE_ALT) { //printf ("alt + :%c: | %d\n",key,key);
		switch (key) {
			case 'a':
				options.autoclear = options.autoclear ? 0: 1;
				sprintf (msgbuf2, "Set message autoclear to %s", options.autoclear ? "on":"off");
				addMsg (0, msgbuf2);
				break;
			case 'r':
				needRedraw = needRedraw ? 0 : 1;
				printf ("set needRedraw %s\n", needRedraw ? "On":"Off");
				break;
			case 't':
				lowRedraw = lowRedraw ? 0 : 1;
				printf ("set lowRedraw %s\n", lowRedraw ? "On":"Off");
				break;
			default:
				break;
		}
	}
	else { 
		switch (key) {
		//printf ("key :%c: %d\n\n",key,key);
		// perso
		// special
		case 'a':
			turnAlpha (options.useAlpha?0:1);
			break;
		case 'b':
			setMode (MODE_BROWSER);
			break;
		case 'B':
			//bellPitch -= 10;
			//	sprintf (xsetcmd, "xset b %d %d", bellVol, bellPitch);
			//	printf ("xsetcmd: %s\n", xsetcmd);
			beep (100,160,500);
			break;
		case 'c':
			setMode (MODE_CODEPARSE);
			break;
		case 'C':
			clearMsg ();
			break;
		case 'e':
			setMode (MODE_EDITOR);
			break;
		case 'f':
			options.showFloor = options.showFloor ? 0 : 1;
			sprintf (msgbuf2, "Set showFloor to %s", options.showFloor ? "on":"off");
			break;
		case 'i':
			memset (drawnPrompt->text, 0, MESSAGESIZE);
			sprintf (drawnPrompt->text, "Enter command:");
			memset (drawnPrompt->buffer, 0, MESSAGESIZE);
			drawnPrompt->cursorPosition = 2.0;
			drawnPrompt->drawn = drawnPrompt->drawn ? 0 : 1;
			needRedraw = 1;
			break;
		case 'm':
			if (mode == MODE_MAINMENU) {
				setMode (MODE_MEM);
			}
			else if (mode == MODE_MEM) {
				setMode (MODE_MAINMENU);
			}
			else 
				setMode (MODE_MAINMENU);
			break;
		case 'n':
			//program_nice = (program_nice < 0) ? 19 : -20;
			//nice (prognice);
			//printf ("set nice to %d\n", prognice);
			break;
		case 'N':
			//bellPitch += 10;
			//	sprintf (xsetcmd, "xset b %d %d", bellVol, bellPitch);
			//	printf ("xsetcmd: %s\n", xsetcmd);
			break;
		case 'o':
			once64 = once64 ? 0 : 1;
			printf ("once64 switched to %d\n", once64);
			break;
		case 'p':
			debug.showPosition = debug.showPosition ? 0 :1;
			printf ("debug key p: set showPosition to %d\n", debug.showPosition);
			sprintf (msgbuf2, "camera position: %.2f %.2f %.2f", cam->x, cam->y, cam->z);
			addMsg (0, msgbuf2);
			if (debug.showPosition) {
				if (numpadMode == MODE_CAMERA) {
					sprintf (msgbuf2, "camera position: %.2f %.2f %.2f", cam->x, cam->y, cam->z);
					addMsg (0, msgbuf2);
				}
				else if (numpadMode == MODE_SELECTION) {
					Plane *plane = drawnMesh->plane;
					Point *point = plane->point;
					int found = 0;
					while (1) {
						sprintf (msgbuf2, "plane ID %d:\n", plane->ID);
						addMsg (0, msgbuf2);
						while (1) {
							if (point->selected) {
								found = 1;
								sprintf (msgbuf2, "\tpoint ID %d: %.2f %.2f %.2f", 
												point->ID, point->x, point->y, point->z);
								addMsg (0, msgbuf2);
							}
							if (point->next == NULL) break;
							else point = point->next;
						}
						if (plane->next == NULL) {
							if (!found) {
								sprintf (msgbuf2, "No points selected or set numpadMode to camera");
								addMsg (0, msgbuf2);
							}
							break;
						}
						else {
							plane = plane->next;
							point = plane->point;
						}
					}
				}
			}
			break;
		case 'R':
			//sprintf (xsetcmd, "%s", "xset b 10 400");
			break;
		case 's':
			//printf ("key :%c: %d\n\n",key,key);
			break;
		case 't':
			//messageBox (MB_TYPE_QUESTION, "hello");
			cntx = 0;
			for (;;) {
				++cntx;
				sprintf (msgbuf2, "test %i", cntx);
				addMsg (0, msgbuf2);
				if (cntx > 20) break;
			}
			break;
		case 'T':
			testerDone = testerDone ? 0 : 1;
			printf ("set testerDone to %s\n", testerDone ? "off":"on");
			if (!testerDone) {
				lowRedraw = 1;
				startTester ();
			}
			else lowRedraw = 0;
			break;
		case '+':
			if (!testerDone) {
				options.bellVol += 10;
				sprintf (xsetcmd, "xset b %d %d", options.bellVol, options.bellPitch);
				printf ("xsetcmd: %s\n", xsetcmd);
				break;
			}
			if (numpadMode == MODE_CAMERA) {
				if (cam->step >= cam->maxSpeed) {
					sprintf (msgbuf2, "camera max speed is set to: %.2f", cam->maxSpeed);
					addMsg (0, msgbuf2);
					break;
				}
				increaseCameraSpeed ();
			}
			else if (numpadMode == MODE_SELECTION) {
				if (selectionMovingStep >= selectionMaxMovingStep) {
					sprintf (msgbuf2, "selection max speed is set to: %.2f", selectionMaxMovingStep);
					addMsg (0, msgbuf2);
					break;
				}
				
				selectionMovingStep += 0.025;
				sprintf (msgbuf2, "selection moving step: %.3f", selectionMovingStep);
				addMsg (0, msgbuf2);
			}
			break;
		case '-':
			if (numpadMode == MODE_CAMERA) {
				if (cam->step <= cam->minSpeed) {
					sprintf (msgbuf2, "camera min speed is set to: %.2f", cam->minSpeed);
					addMsg (0, msgbuf2);
					break;
				}
				decreaseCameraSpeed ();
			}
			else if (numpadMode == MODE_SELECTION) {
				selectionMovingStep -= 0.005;
				if (selectionMovingStep <= 0.000)
					selectionMovingStep = 0.005;
				else {
					sprintf (msgbuf2, "selection moving step: %.3f", selectionMovingStep);
					addMsg (0, msgbuf2);
				}
			}
			break;
		}
	}
	if (numpadMode == MODE_CAMERA) {
		switch (key) {
			case '9':
				movingCamUD = 1;
				break;
			case '3':
				movingCamUD = -1;
				break;
			case '8':
				lookUD = 1;
				break;
			case '2':
				lookUD = -1;
				break;
			case '4':
				lookLR = -1;
				break;
			case '6':
				lookLR = 1;
				break;
			default:
				break;
		}
	}
	else if (numpadMode == MODE_SELECTION) {
		switch (key) {
			case '9':
				movingSelectionIO = -1;
				break;
			case '3':
				movingSelectionIO = 1;
				break;
			case '8':
				movingSelectionUD = 1;
				break;
			case '2':
				movingSelectionUD = -1;
				break;
			case '4':
				movingSelectionLR = -1;
				break;
			case '6':
				movingSelectionLR = 1;
				break;
			default:
				break;
		}
	}
}

void normalKeysUp (unsigned char key, int x, int y) {
	//if (!drawnPrompt->drawn) glutSetKeyRepeat (GLUT_KEY_REPEAT_ON);
	if (numpadMode == MODE_CAMERA) {
		switch (key) {
			case '9':
				movingCamUD = 0;
				break;
			case '3':
				movingCamUD = 0;
				break;
			case '8':
				lookUD = 0;
				break;
			case '2':
				lookUD = 0;
				break;
			case '4':
				lookLR = 0;
				break;
			case '6':
				lookLR = 0;
				break;
			default:
				break;
		}
	}
	else if (numpadMode == MODE_SELECTION) {
		switch (key) {
			case '9':
				movingSelectionIO = 0;
				break;
			case '3':
				movingSelectionIO = 0;
				break;
			case '8':
				movingSelectionUD = 0;
				break;
			case '2':
				movingSelectionUD = 0;
				break;
			case '4':
				movingSelectionLR = 0;
				break;
			case '6':
				movingSelectionLR = 0;
				break;
			default:
				break;
		}
	}
}

void specialKeysDown (int key, int x, int y) {
	needRedraw = 1;
	//printf ("key: %d :%c:\n", key, key);
	if (mode == MODE_RACE) {
		switch (key) {
			case GLUT_KEY_UP:
				firstCar.speed += 0.01;
				printf ("car speed: %f\n", firstCar.speed);
				break;
			case GLUT_KEY_DOWN:
				firstCar.speed -= 0.01;
				printf ("car speed: %f\n", firstCar.speed);
				break;
			case GLUT_KEY_LEFT:
		//		movingCamLR = -1;
				break;
			case GLUT_KEY_RIGHT:
		//		movingCamLR = 1;
				break;
			default:
				break;
		}
	}
	else {
		switch (key) {
				case GLUT_KEY_F5:
					frameSleepTime -= 10000;
					if (frameSleepTime < 0) { printf ("frameSleepTime < zero\n"); frameSleepTime = 0; }
					printf ("frameSleepTime = %d\n", frameSleepTime);
					break;
				case GLUT_KEY_F6:
					frameSleepTime += 10000;
					printf ("frameSleepTime = %d\n", frameSleepTime);
					break;
			case GLUT_KEY_UP:
				movingCamIO = 1;
				break;
			case GLUT_KEY_DOWN:
				movingCamIO = -1;
				break;
			case GLUT_KEY_LEFT:
				movingCamLR = -1;
				break;
			case GLUT_KEY_RIGHT:
				movingCamLR = 1;
				break;
			default:
				break;
		}
	}
}

void specialKeysUp (int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_F1:
			hudShowShortcuts ();
			break;
		case GLUT_KEY_F4:
			exitNexus (STATE_EVENT, STATE_OK, __FUNCTION__);
			break;
		case GLUT_KEY_F5:
			break;
		case GLUT_KEY_F8:
			break;
		case GLUT_KEY_UP:
			movingCamIO = 0;
			if (debug.showPosition) {
				printf ("camera position: %.2f %.2f %.2f\n", cam->x, cam->y, cam->z);
			}
			break;
		case GLUT_KEY_DOWN:
			movingCamIO = 0;
			if (debug.showPosition) {
				printf ("camera position: %.2f %.2f %.2f\n", cam->x, cam->y, cam->z);
			}
			break;
		case GLUT_KEY_LEFT:
			movingCamLR = 0;
			if (debug.showPosition) {
				printf ("camera position: %.2f %.2f %.2f\n", cam->x, cam->y, cam->z);
			}
			break;
		case GLUT_KEY_RIGHT:
			movingCamLR = 0;
			if (debug.showPosition) {
				printf ("camera position: %.2f %.2f %.2f\n", cam->x, cam->y, cam->z);
			}
			break;
		default:
			break;
	}
}

void hoverObj (int x, int y) {
	float aspect;
	GLint hits, viewport[4] = {0,0,0,0};
	int xx = 0;
	while (1) {
		selectBuf[xx] = 0;
		++xx;
		if (xx >= SELECT_BUFFER_SIZE) break;
	}

	glSelectBuffer (SELECT_BUFFER_SIZE, selectBuf);
	glGetIntegerv (GL_VIEWPORT, viewport);

	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();

	glRenderMode (GL_SELECT);
	glLoadIdentity ();
	gluPickMatrix ((double)x, (double)viewport[3]/1.0 - (double)y, 50,50, viewport);
	aspect = (float)viewport[2] / (float)viewport[3];
	gluPerspective (45.0, aspect, 0.01, 1000.0);
	rendering = 0; // prevents calling setOrthographicProjection() for the 2D text
	render ();
	rendering = 1;

	hits = glRenderMode (GL_RENDER);
	
	if (hits > 0) {
		Plane *plane;
		Point *point;
		int cnt = 0;	// hit count
		int type;		// plane or point?
		if (once64) printf ("hit buf ID:");
		// for all hits
		while (1) {
			if (once64) printf (" %d", selectBuf[cnt+3]);
			type = typeByID (selectBuf[cnt+3]);
			if (type == SID_TYPE_PLANE) {
				plane = planeByID (selectBuf[cnt+3]);
				if (plane != NULL) {
					if (hoveringPlane) {
						Plane *plane2 = planeByID (lastHoveredPlaneID);
						if (plane == NULL) {
							if (debug.events) printf ("events:hoverObj(): no lastPlaneID found\n");
						}
						else {
							hoverPlane (plane2, 0);
							plane2 = NULL;
						}
					}
					
					lastHoveredPlaneID = plane->ID;
					hoveredPlaneID = plane->ID;
					hoverPlane (plane, 1);
					hoveringPlane = 1;
				}
				else {
					if (debug.events) printf ("events:hoverObj(): no plane ID found\n");
				}

				/*if (plane->ID != lastHoveredPlaneID && hoveringPlane) {
					Plane *plane2 = planeByID (lastHoveredPlaneID);
					if (plane2 != NULL) {
						hoveredPlaneID = plane->ID;
						hoverPlane (plane2);
						hoveringPlane = 0;
					}
				}*/
			}
			else if (type == SID_TYPE_POINT) {
				if (hoveringPoint) {
					point = pointByID (lastHoveredPointID);
					if (point == NULL) {
						if (debug.events) printf ("events:hoverObj(): no last point ID\n");
					}
					else
						hoverPoint (point, 0);
				}
				point = pointByID (selectBuf[cnt+3]);
				if (point == NULL) {
					if (debug.events) printf ("events:hoverObj(): no point ID found\n");
				}
				else {
					hoveredPointID = point->ID;
					hoverPoint (point, 1);
					hoveringPoint = 1;
				}
			}
			else { if (debug.events) printf ("events:hoverObj(): SID type not defined\n"); }
			cnt += 4;
			if (cnt/4 >= hits) break;
		}
		if (once64) {
			printf (" total: %d hits\n", hits);
			once64 = 0;
		}
	}
	else {
		if (hoveringPlane) {
			hoveringPlane = 0;
			if (curHoveredPlane != NULL) {
				hoverPlane (curHoveredPlane, 0);
				curHoveredPlane = NULL;
			}
		}
		if (hoveringPoint) {
			hoveringPoint = 0;
			if (curHoveredPoint != NULL) {
				hoverPoint (curHoveredPoint, 0);
				curHoveredPoint = NULL;
			}
		}
	}
	
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
}

void selectObj (int x, int y) {
	float aspect;
	GLint hits, viewport[4] = {0,0,0,0};
	
	glSelectBuffer (SELECT_BUFFER_SIZE, selectBuf);
	glGetIntegerv (GL_VIEWPORT, viewport);
	
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glRenderMode (GL_SELECT);
	glLoadIdentity ();
	gluPickMatrix ((double)x, (double)viewport[3]/1.0 - (double)y, 0.2,0.2, viewport);
	aspect = (float)viewport[2] / (float)viewport[3];
	gluPerspective (45.0, aspect, 0.01, 1000.0);
	rendering = 0; // prevents calling setOrthographicProjection() for the 2D text
	render ();
	rendering = 1;
	
	hits = glRenderMode (GL_RENDER);
	
	if (hits > 0) {
		printf ("%d hits:", hits);
		int cnt = 0;
		Plane *tmpPlane;
		Point *tmpPoint;
		while (1) {
			printf (" #%d", selectBuf[cnt+3]);
			if (typeByID (selectBuf[cnt+3]) == SID_TYPE_PLANE) {
				tmpPlane = planeByID (selectBuf[cnt+3]);
				if (tmpPlane != NULL)
					selectPlane (tmpPlane);
			}
			else {
				tmpPoint = pointByID (selectBuf[cnt+3]);
				if (tmpPoint != NULL) {
					selectPoint (tmpPoint);
					curSelectedPoint = tmpPoint;
				}
			}
			cnt += 4;
			if (cnt/4 >= hits) break;
		} fflush (NULL);
	}
	
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
}

void selectObjs (int x, int y) {
	float aspect;
	GLint hits, viewport[4] = {0,0,0,0};
	
	int xx = 0;
	while (1) {
		selectBuf[xx] = 0;
		++xx;
		if (xx >= SELECT_BUFFER_SIZE) break;
	}
	
	glSelectBuffer (SELECT_BUFFER_SIZE, selectBuf);
	glGetIntegerv (GL_VIEWPORT, viewport);
	
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glRenderMode (GL_SELECT);
	glLoadIdentity ();
	
 	gluPickMatrix ( 	((double)lastMouseX-(double)firstMouseX)/2.0 + (double)firstMouseX, 
 		(double)winHeight/1.0 - ((double)lastMouseY-(double)firstMouseY)/2.0 - (double)firstMouseY, 
 		 (double)lastMouseX - (double)firstMouseX, 
		  (double)lastMouseY - (double)firstMouseY,
		viewport);
	aspect = (float)viewport[2] / (float)viewport[3];
	gluPerspective (45.0, aspect, 0.01, 1000.0);
	
	rendering = 0; // prevents calling setOrthographicProjection () for the 2D text
	render ();
	rendering = 1;
	
	hits = glRenderMode (GL_RENDER);
	
	if (hits > 0) {
		int cnt = 0;
		while (1) {
			if (typeByID (selectBuf[cnt+3]) == SID_TYPE_POINT) {
				Point *tmpPoint = pointByID (selectBuf[cnt+3]);
				if (tmpPoint != NULL) {
					selectPoint (tmpPoint);
					curSelectedPoint = tmpPoint;
				}
				else break;
			}
			cnt += 4;
			if (x/4 >= hits) break;
		}
	}
	
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
}

void selectMenu (int x, int y) {
	float aspect;
	GLint hits, viewport[4] = {0,0,0,0};
	printf ("selectMenu()\n");
	
	setOrthographicProjection ();
	glSelectBuffer (SELECT_BUFFER_SIZE, selectBuf);
	glGetIntegerv (GL_VIEWPORT, viewport);
	
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glRenderMode (GL_SELECT);
	glLoadIdentity ();
	
	gluPickMatrix ((double)x, (double)viewport[3] - (double)y, 2.0,2.0, viewport);
	aspect = (float)viewport[2] / (float)viewport[3];
	gluOrtho2D (0.0, (double)winWidth, 0.0, (double)winHeight);
	glScalef (1.0,-1.0, 1.0);
	glTranslatef (0.0, -winHeight, 0.0);
	
	rendering = 0; // prevents calling setOrthographicProjection() 
	drawMenu ();
	rendering = 1;
	
	hits = glRenderMode (GL_RENDER);
	setPerspectiveProjection ();
	
	if (hits == 0) {
		menuHideAll ();
		menuLastSelectedEntry = NULL;
	}
	else if (hits == 1) {
		//printf ("selectMenu(): hit ID %d\n", selectBuf[3]);
		struct Entry *entry = entryByID2 (__FILE__,__LINE__,"selectMenu()",selectBuf[3]);
		if (entry == NULL) {
			sprintf (msgbuf2, "selectMenu(): Found no entry for ID %d!", selectBuf[3]);
			addMsg (0, msgbuf2);
		}
		else {
			menuLastSelectedEntry = entry;
			if (entry->type == ENTRY_TYPE_SUBMENU) {
				showSubmenu (entry);
			}
			else if (entry->type == ENTRY_TYPE_ACTION) {
				switch (selectBuf[3]) {
				case ENTRY_EXIT:
					printf ("\n");
					exitNexus (STATE_EVENT, STATE_OK, __FUNCTION__);
					break;
				case ENTRY_OPTIONS_SHORTCUTS:
					hudShowShortcuts ();
					break;
				case ENTRY_OPTIONS_CLEARMSG:
					clearMsg ();
					break;
				case ENTRY_OPTIONS_AUTOCLEAR:
					options.autoclear = (options.autoclear) ? 0:1;
					//entry = entryByID2 (__FILE__,__LINE__,"selectMenu()",ENTRY_OPTIONS_AUTOCLEAR);
					//if (entry != NULL) entry->enabled = entry->enabled ? 0 : 1;
					entry->enabled = options.autoclear;
					break;
				case ENTRY_OPTIONS_AUTOSAVESTATE:
					options.autoSaveState = options.autoSaveState ? 0 : 1;
					//entry = entryByID (ENTRY_OPTIONS_AUTOSAVESTATE);
					//if (entry != NULL) entry->enabled = entry->enabled ? 0 : 1;
					entry->enabled = options.autoSaveState;
					break;
				case ENTRY_OPTIONS_SHOWCLOCK:
					options.showClock = options.showClock?0:1;
					//entry = entryByID (ENTRY_OPTIONS_SHOWCLOCK);
					//if (entry != NULL) entry->enabled = entry->enabled ? 0 : 1;
					entry->enabled = options.showClock;
					break;
				case ENTRY_OPTIONS_SHOWFPS:
					options.showFPS = options.showFPS ? 0 : 1;
					//entry = entryByID (ENTRY_OPTIONS_SHOWFPS);
					//if (entry != NULL) entry->enabled = entry->enabled ? 0 : 1;
					entry->enabled = options.showFPS;
					break;
				case ENTRY_OPTIONS_SHOWFLOOR:
					options.showFloor = options.showFloor ? 0 : 1;
					//entry = entryByID (ENTRY_OPTIONS_SHOWFLOOR);
					//if (entry != NULL) entry->enabled = entry->enabled ? 0 : 1;
					entry->enabled = options.showFloor;
					break;
				case ENTRY_OPTIONS_DEBUG:
					options.debug = options.debug ? 0 : 1;
					//entry = entryByID (ENTRY_OPTIONS_DEBUG);
					//if (entry != NULL) entry->enabled = entry->enabled ? 0 : 1;
					entry->enabled = options.debug;
					break;
				case  ENTRY_OPTIONS_SHOWSENSORS:
					options.showSensors = options.showSensors ? 0 : 1;
					entry->enabled = options.showSensors;
					//entry = entryByID (ENTRY_OPTIONS_SHOWSENSORS);
					//if (entry != NULL) entry->enabled  = options.showSensors;
					//if (options.showSensors) {
					//	pthread_create (&sensThread, NULL, sensThreadFunc, NULL);
					//	pthread_detach (sensThread);
					//}
					break;
				case ENTRY_OPTIONS_USEALPHA:
					turnAlpha (options.useAlpha?0:1);
					entry->enabled = options.useAlpha;
					break;
				/*case ENTRY_OPTIONS_VERBOSE_L0:
					options.verbose = 0;
					addMsg (4, "Set verbose level to OFF");
					break;
				case ENTRY_OPTIONS_VERBOSE_L1:
					options.verbose = 1;
					addMsg (4, "Set verbose level to 1");
					break;
				case ENTRY_OPTIONS_VERBOSE_L2:
					options.verbose = 2;
					addMsg (4, "Set verbose level to 2");
					break;
				case ENTRY_OPTIONS_VERBOSE_L3:
					options.verbose = 3;
					addMsg (4, "Set verbose level to 3");
					break;
				case ENTRY_OPTIONS_VERBOSE_L4:
					options.verbose = 4;
					addMsg (4, "Set verbose level to 4");
					break;
				*/ //case ENTRY_SUBMENU_FONT:
				//	showSubentry (entryByID (ENTRY_SUBMENU_FONT));
				//	break;
				/*case ENTRY_FONT_9x15:
					HUD_FONT = GLUT_BITMAP_9_BY_15;
					break;
				case ENTRY_FONT_8x13:
					HUD_FONT = GLUT_BITMAP_8_BY_13;
					break;
				case ENTRY_FONT_TIMES_ROMAN_10:
					HUD_FONT = GLUT_BITMAP_TIMES_ROMAN_10;
					break;
				case ENTRY_FONT_TIMES_ROMAN_24:
					HUD_FONT = GLUT_BITMAP_TIMES_ROMAN_24;
					break;
				case ENTRY_FONT_HELVETICA_10:
					HUD_FONT = GLUT_BITMAP_HELVETICA_10;
					break;
				case ENTRY_FONT_HELVETICA_12:
					HUD_FONT = GLUT_BITMAP_HELVETICA_12;
					break;
				case ENTRY_FONT_HELVETICA_18:
					HUD_FONT = GLUT_BITMAP_HELVETICA_18;
					break;*/
				case ENTRY_MODE_EDITOR:
					setMode (MODE_EDITOR);
					break;
				case ENTRY_MODE_MEM:
					setMode (MODE_MEM);
					break;
				case ENTRY_MODE_CODEPARSE:
					setMode (MODE_CODEPARSE);
					break;
				case ENTRY_MODE_BROWSER:
					setMode (MODE_BROWSER);
					break;
				case ENTRY_MODE_RACE:
					setMode (MODE_RACE);
					break;
				case ENTRY_MODE_BUFFER:
					setMode (MODE_BUFFER);
					break;
				case ENTRY_EDITOR_DEBUG_SHOWIDS:
					debug.showids = debug.showids ? 0 : 1;
					//entry = entryByID (ENTRY_EDITOR_DEBUG_SHOWIDS);
					//if (entry != NULL) entry->enabled = 1;
					entry->enabled = entry->enabled ? 0 : 1;
					sprintf (msgbuf2, "Set debug.showids %s", debug.showids ? "on":"off");
					addMsg (4, msgbuf2);
					break;
				case ENTRY_EDITOR_DEBUG_SHOWPOSITION:
					debug.showPosition = debug.showPosition ? 0 : 1;
					//entry = entryByID (ENTRY_EDITOR_DEBUG_SHOWPOSITION);
					//if (entry != NULL) entry->enabled = 1;
					entry->enabled = entry->enabled ? 0 : 1;
					sprintf (msgbuf2, "Set debug.showPosition %s", debug.showPosition ? "on":"off");
					addMsg (4, msgbuf2);
					break;
				case ENTRY_EDITOR_DEBUG_SHOWPOINT:
					debug.showPoint = debug.showPoint ? 0 : 1;
					//entry = entryByID (ENTRY_EDITOR_DEBUG_SHOWPOINT);
					//if (entry != NULL) entry->enabled = 1;
					entry->enabled = entry->enabled ? 0 : 1;
					sprintf (msgbuf2, "Set debug.showPoint %s", debug.showPoint ? "on":"off");
					addMsg (4, msgbuf2);
					break;
				case ENTRY_DEBUG_DRAWING:		// argh doesnt work damn it
					printf ("select drawing debug\n");
					debug.drawing = debug.drawing?0:1;
					entry->enabled = debug.drawing;
					if (debug.drawing) addMsg (0, "$=waiting ~=threadLaunch @=bufferFlush");
					break;
				case ENTRY_EDITOR_MAINMENU:
					setMode (MODE_MAINMENU);
					break;
				case ENTRY_EDITOR_HELP_SHORTCUTS:
					hudShowShortcuts ();
					break;
				case ENTRY_EDITOR_ADD_POINT:
					if (curSelectedPoint != NULL) {
						Plane *tmpPlane2 = planeByID (curSelectedPoint->ID);
						if (tmpPlane2)
							addPointAfter (tmpPlane2, curSelectedPoint);
						else printf ("events ENTRY_EDITOR_ADD_POINT: no proper plane from pointID %d\n",
									curSelectedPoint->ID);
					}
					else addMsg (0, "Select a point first");
					break;
				case ENTRY_EDITOR_ADD_PLANE:
					addPlane (drawnMesh);
					Plane *tmpPlane2 = drawnMesh->lastPlane;
					addPoint (tmpPlane2, 2.0, 3.0, 0.0);
					addPoint (tmpPlane2, 3.0, 3.0, 0.0);
					addPoint (tmpPlane2, 3.0, 2.0, 0.0);
					break;
				case ENTRY_EDITOR_ADD_MESH:
					addMsg (0, "Not implemented");
					break;
				case ENTRY_EDITOR_FILE_LOAD:
					printf ("events(): shoudl call loadMesh(?)\n");
					break;
				case ENTRY_EDITOR_FILE_SAVE:
					saveMesh (drawnMesh);
					break;
				case ENTRY_EDITOR_FILE_LOAD_DEFAULTMESH:
					//addMsg (0, "Mesh loading disabled");
					evmesh = loadMesh ("DefaultMesh");
					//showMeshTree (evmesh);
					if (evmesh == NULL) break;
					drawnMesh = evmesh;
					needRedraw = 1;
					break;
				case ENTRY_EDITOR_FILE_LOAD_ARMYTRUCK:
					evmesh = loadMesh ("ArmyTruck");
					if (evmesh == NULL) break;
					drawnMesh = evmesh;
					needRedraw = 1;
					break;
				default:
					break;
				}
			}
			else {
				addMsg (0, "selectMenu(): this menu has no type");
			}
		}
	}
	else if (hits > 1) {
		sprintf (msgbuf, "selectMenu(): GOT %d HITS!\n", hits);
		addMsg (5, msgbuf);
	}
	
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
}

int tmpID = 0;
void hoverMenu (int x, int y) {
	float aspect;
	GLint hits, viewport[4] = {0,0,0,0};
	
	setOrthographicProjection ();
	glSelectBuffer (SELECT_BUFFER_SIZE, selectBuf);
	glGetIntegerv (GL_VIEWPORT, viewport);
	
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	
	glRenderMode (GL_SELECT);
	glLoadIdentity ();
	
	gluPickMatrix ((double)x, (double)viewport[3] - (double)y, 2.0,2.0, viewport);
	aspect = (float)viewport[2] / (float)viewport[3];
	gluOrtho2D (0.0, (double)winWidth, 0.0, (double)winHeight);
	glScalef (1.0,-1.0, 1.0);
	glTranslatef (0.0, -winHeight, 0.0);
	
	rendering = 0; // prevents calling setOrthographicProjection() for the 2D text
	drawMenu ();
	rendering = 1;
	
	hits = glRenderMode (GL_RENDER);
	setPerspectiveProjection ();
	
	if (hits > 0 && !hoveringMenu) {
		hoveringMenu = 1;
	}
	if (hits == 0) {
		if (hoveringMenu) {
			hoveringMenu = 0;
		}
	}
	
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
}

void mouseFunc (int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		selecting = 1;
		firstMouseX = x;
		firstMouseY = y;
		lastMouseX = x;
		lastMouseY = y;
		lastClick = glutGet (GLUT_ELAPSED_TIME);
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		selecting = 0;
		int curClickTmp = glutGet (GLUT_ELAPSED_TIME);
		if (mainMenuList.rootMenu->drawn && curClickTmp <= lastClick + 250) selectMenu (x, y);
		else {
			if (curClickTmp <= lastClick + 500) {
				selectObj (x, y);
			}
			else {
				selectObjs (x, y);
			}
		}
		lastMouseX = 0;
		lastMouseY = 0;
	}
	else if (button == 3) {
		if (mode == MODE_BROWSER) {
			rootFileTree->x += 0.1;
		}
	}
	else if (button == 4) {
		if (mode == MODE_BROWSER) {
			rootFileTree->x -= 0.1;
		}
	}
	//if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
	//	showMenu (x, y);
	//}
	return;
}

void mouseMotionFunc (int x, int y) {
	if (selecting) {
		lastMouseX = x;
		lastMouseY = y;
	}
}

pthread_t menuHideThread;
void *menuHideThreadFunc (void *arg) {
	sleep (3);
	mainMenuList.rootMenu->drawn = 0;
	return NULL;
}

void mousePassiveMotionFunc (int x, int y) {
	if (!mainMenuList.rootMenu->drawn && x <= 20 
	&& y > (winHeight -40.0) - (mainMenuList.rootMenu->lastEntry->rank*ENTRY_SIZEY)) {
		mainMenuList.rootMenu->drawn = 1;
	}
	if (mainMenuList.rootMenu->drawn && x <= ENTRY_SIZEX && 
		   y < winHeight && 
		   y > winHeight - (mainMenuList.rootMenu->lastEntry->rank*ENTRY_SIZEY)) {
		hoverMenu (x, y);
		return;
	}
	//else if (menu.drawn) {
	//	pthread_create (&menuHideThread, NULL, menuHideThreadFunc, NULL);
	//	pthread_detach (menuHideThread);
	//}
	else if (mode == MODE_EDITOR) {
		hoverObj (x, y);
	}
	//
}

// These three handle anything when the window focus change
//
//
void entryFunc (int state) {
	//printf ("entryFunc(): keyboard repeat state = %d\n", state);
	if (state == 1) {
		//glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
		//printf ("entryFunc(): set keyboard repeat off\n");
	}
	else {
		//glutSetKeyRepeat (GLUT_KEY_REPEAT_ON);
		//printf ("entryFunc(): set keyboard repeat on\n");
	}
}

void visibilityFunc (int state) {
	//printf ("visibilityFunc(): keyboard repeat state = %d\n", state);
	/*if (state == 1) {
		glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
		printf ("set keyboard repeat off\n");
	}
	else {
		glutSetKeyRepeat (GLUT_KEY_REPEAT_ON);
		printf ("set keyboard repeat on\n");
	}*/
}

void windowFunc (int status) {
	//printf ("windowFunc(): window status = %d\n", status);
	if (status != 1) {
		//glutSetKeyRepeat (GLUT_KEY_REPEAT_ON);
		//printf ("windowFunc(): set keyboard repeat on\n");
	}
	else {
		//glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
		//printf ("windowFunc(): set keyboard repeat off\n");
	}
}
