#ifndef DRAWING_H
#define DRAWING_H 1

#include <GL/glut.h>

#include "memoryViewer.h"
#include "menu.h"
#include "mesh.h"
#include "Ntype.h"
#include "objects.h"

#include <X11/Xlib.h>

//extern Display *display;
extern GC gc;

extern int rendering;	// This flag is used in events.c :: selectObj to prevent
						// rasterization clashes. Without it, selection is done improperly.
						// Used by hud2dText() unless what the hud text blinks
extern mbool needRedraw;
extern mbool lowRedraw;
extern int frame;
extern short fps;
extern unsigned int frameSleepTime;
extern float codeDelta;
extern float delta;
extern float deltaStep;
extern float tmpred1, tmpred2;
extern float ratio;// stuff for the fps meter shown in the upper-right corner
extern float tx, ty, tz;	// testing
extern int drawSphere;
extern sMenu *drawnMenu;

struct Func {
	void (*func) (void);
	int ab;
};
extern struct Func func;

//extern GLubyte fire[128];
/*GLubyte fire[128] = { 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0xc0,
 0x00, 0x00, 0x01, 0xf0,
 0x00, 0x00, 0x07, 0xf0,
 0x0f, 0x00, 0x1f, 0xe0,
 0x1f, 0x80, 0x1f, 0xc0,
 0x0f, 0xc0, 0x3f, 0x80,	
 0x07, 0xe0, 0x7e, 0x00,
 0x03, 0xf0, 0xff, 0x80,
 0x03, 0xf5, 0xff, 0xe0,
 0x07, 0xfd, 0xff, 0xf8,
 0x1f, 0xfc, 0xff, 0xe8,
 0xff, 0xe3, 0xbf, 0x70, 
 0xde, 0x80, 0xb7, 0x00,
 0x71, 0x10, 0x4a, 0x80,
 0x03, 0x10, 0x4e, 0x40,
 0x02, 0x88, 0x8c, 0x20,
 0x05, 0x05, 0x04, 0x40,
 0x02, 0x82, 0x14, 0x40,
 0x02, 0x40, 0x10, 0x80, 
 0x02, 0x64, 0x1a, 0x80,
 0x00, 0x92, 0x29, 0x00,
 0x00, 0xb0, 0x48, 0x00,
 0x00, 0xc8, 0x90, 0x00,
 0x00, 0x85, 0x10, 0x00,
 0x00, 0x03, 0x00, 0x00,
 0x00, 0x00, 0x10, 0x00 };*/

void initDrawing (void);
// 3D
void idleFunc (void);
//void initFloor (void);
void turnAlpha (mbool On_Off);
void reshape (int width, int height);
void render2 (void);
void PPdebugCountX (void);
void render (void);
void renderBrowser (void);

extern int codeParse_RootDisplayListNode_ID;
extern struct DisplayListNode *codeParse_RootDisplayListNode;
extern void (*renderingModeFunc) (void);
void renderCodeParse (void);
void renderEditor (void);
//void renderFloor (void);	The floor is now a displayList
void renderGame (void);
void renderMainmenu (void);
void renderMem (void);
void renderRace (void);
void drawVerticalMeter (VerticalMeter *meter);

// 2D
void drawBrowser (void);
void drawMesh (sMesh *mesh);
//void drawObject (struct Object *obj);
void drawPrompt (void);
void drawSelectBox (void);

#endif /* DRAWING_H */
