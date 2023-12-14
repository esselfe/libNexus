#ifndef EVENTS_H
#define EVENTS_H 1

#include <GL/glut.h>
#include <mesh.h>

#define SELECT_BUFFER_SIZE 4*256

extern Plane *curSelectedPlane, *lastSelectedPlane, *curHoveredPlane;
extern Point *curSelectedPoint, *lastcurSelectedPoint, *curHoveredPoint;
extern int lastHoveredPlaneID;
extern int hoveredPlaneID;
extern int lastHoveredPointID;
extern int hoveredPointId;
extern struct Entry *curHoveredEntry;
extern struct Entry *menuLastSelectedEntry;
extern mbool selecting;
//extern mbool hoveringObj;
extern mbool hoveringPlane;
extern mbool hoveringPoint;
extern mbool hoveringMenu;
extern int lastClick;
extern int firstMouseX, firstMouseY;
extern int lastMouseX, lastMouseY;
extern int cursorX, cursorY;
// extern int markX1, markY1, markX2, markY2, markX3, markY3, markX4, markY4;
extern GLuint selectBuf [SELECT_BUFFER_SIZE];
extern int once64;

//void normalKeys (unsigned char key, int x, int y);
void normalKeysDown (unsigned char key, int x, int y);
void normalKeysUp (unsigned char key, int x, int y);
void specialKeysDown (int key, int x, int y);
void specialKeysUp (int key, int x, int y);
void hoverObj (int x, int y);
void selectObj (int x, int y);
void selectObjs (int x, int y);
void hoverMenu (int x, int y);
void selectMenu (int x, int y);
void mouseFunc (int button, int state, int x, int y);
void mouseMotionFunc (int x, int y);
void mousePassiveMotionFunc (int x, int y);
void entryFunc (int state);
void visibilityFunc (int state);
void windowFunc (int status);

#endif /* EVENTS_H */
