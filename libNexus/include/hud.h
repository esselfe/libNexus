#ifndef GUI_H
#define GUI_H 1

#include <time.h>

#include <GL/glut.h>

#include <Ntype.h>

//extern const float hudAlpha;
#define hudAlpha 0.8
extern float hudTextColor[4], hudTextOnceColor[4];
extern mbool hudTextOnceColorFlag;	// Used
extern void *HUD_FONT;

// stuff for the clock in the lower-right corner of the screen
time_t mtime;
struct tm *mtm;
extern float bottomTextBackgroundColor[4];
extern char hudClockText [80];
extern time_t hudClockTime;

extern char fpsText [10];
extern short hudUpdateFPSDelay;

struct sHud_TextPlane {
	float x, y, w, h;
	char *text;
};

struct sHudObject_Entry {
	struct sHudObjectEntry *prev, *next;
	int id;
	struct sHud_TextPlane textPlane;
};

struct sHudObject_EntryList {
	struct sHudObject_EntryList *prev, *next;
	Position position;
	Color color;	// particular user settings for distinctions
	char *label;
	struct sHudObject_Entry *rootEntry, *lastEntry;
} *rootEntryList, *lastEntryList;

struct Button {
	int ID;
	char *text;
	float x, y, width, height;
	struct Buttons *prev, *next;
};

struct MessageBox {
	int ID;
	int type;
	char *text;
	char *input;
	float x, y, width, height;
	short yes, no, cancel;
	struct Button *firstButton, *lastButton;
};

struct MessageBoxQueue {
	int queueID;
};

void initHud (void);

int messageBox (int type, char *text);

void setOrthographicProjection (void);	// This is for proper 2D rendering
void setPerspectiveProjection (void);	// idem

void hudTextSetOnceColor (float red, float green, float blue, float alpha);
void hudTextSetColor (float red, float green, float blue, float alpha);
void hud2dText (int x, int y, char *text2);
void hud3dText (char *text2, float x, float y, float z);

void hudUpdateClock (void);
void hudUpdateFPS (void);

void NLdrawHud (void);
void drawMsg (void);
void drawMessageBox (void);
void drawClock (void);
void drawFPS (void);
void drawSensors (void);

struct sHudObject_EntryList *hudNewEntryList (char *label,
											  float x, float y, float z,
			 float red, float green, float blue, float alpha);
void hudShowShortcuts (void);

#endif /* GUI_H */
