#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <pthread.h>

#include <GL/glut.h>

#include "camera.h"
#include "commands.h"
#include "config.h"
#include "osdata.h"
#include "drawing.h"
#include "events.h"
#include "hud.h"
#include "memfiller.h"
#include "menu.h"
#include "messages.h"
#include "libNexus.h"
#include "Ntype.h"
#include "state.h"
#include "tester.h"
#include "threading.h"
#include "util.h"

mbool messageBoxPending = 0;

float hudBottomTextBackgroundColor[4] = {0.3, 0.55, 0.4, 0.6};
float hudTextColor[4] = {0.7, 0.8, 0.7, 0.6};
float hudTextOnceColor[4] = {0.3, 0.7, 0.3, 0.6};
float hudMsgTextColor[4] = {0.7, 0.8, 0.7, 0.5};
float hudMsgTextOnceColor[4] = {0.3, 0.7, 0.3, 0.5};
mbool hudTextOnceColorFlag = 0;
void *HUD_FONT = GLUT_BITMAP_HELVETICA_12;

char hudClockText[80];
time_t hudClockTime;
time_t mtime;
struct tm *mtm;
char *wday[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char *mday[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

char fpsText [10];
short hudUpdateFPSDelay = 1;	// Delay in seconds, used in timerThreadFunc()

struct sHudObject_EntryList *rootEntryList, *lastEntryList;

/*GLUT_BITMAP_8_BY_13;
 GLUT_BITMAP_9_BY_15
 GLUT_BITMAP_TIMES_ROMAN_10
 GLUT_BITMAP_TIMES_ROMAN_24
 GLUT_BITMAP_HELVETICA_10
 GLUT_BITMAP_HELVETICA_12
 GLUT_BITMAP_HELVETICA_18*/

void initHud (void) {
	msgbuf = malloc (MESSAGESIZE);
	msgbuf2 = malloc (MESSAGESIZE);
	line = malloc (128);
	line2 = line;
	memset (msgbuf, 0, MESSAGESIZE-1);
	memset (msgbuf2, 0, MESSAGESIZE-1);
	memset (line, 0, 127);
	
	//mb.type = MB_TYPE_QUESTION;
	//mb.text = malloc (MESSAGESIZE);
	//mb.input = malloc (MESSAGESIZE);
	//mb.yes = 0;
	//mb.no = 0;
	//mb.cancel = 1;
	
	
	/* ***Init entryList */
	/*
	struct sHudObject_EntryList *list = &rootEntryList;
	list->prev = NULL;
	list->next = NULL;
	typeSetPosition (&list->position, 0.2, 0.4, 0.9);
	typeSetColor (&list->color, 0.0, 0.4, 0.2, 0.35);
	list->label = malloc (8);
	sprintf (list->label, "mainMenu");
	list->rootEntry = NULL;
	list->lastEntry = NULL;
	lastEntryList = list;*/
	
	/*hudNewEntryList (char *label,
	float x, float y, float z,
	float red, float green, float blue, float alpha);*/
	rootEntryList = hudNewEntryList ("mainMenu", 1.0, 1.0, 0.9, 0.0, 0.6, 0.3,  0.35);
	
	/* ***Init messages */
	firstmsg.text = malloc (MESSAGESIZE);
	sprintf (firstmsg.text, "Initialized the message board");
	msgPending = 1;
	firstmsg.rank = 1;
	firstmsg.prev = NULL;
	firstmsg.next = NULL;
	curmsg = NULL;
	lastmsg = &firstmsg;
	
	/* ***Set useful data before rendering */
	hudUpdateClock ();
	hudUpdateFPS ();
}
/*
void parsePrompt (unsigned char key) {
	//printf ("parsePrompt(): key: %d :%c:\n", key, key);
	if (key == 8) { // Backspace
		char *cc = drawnPrompt->buffer + strlen (drawnPrompt->buffer) - 1;
		*cc = '\0';
		return;
	}
	if (key == 13) {	// Enter
		drawnPrompt->drawn = 0;
		if (strcmp ("qw", drawnPrompt->buffer) == 0) {
			exitNexus (STATE_EVENT, STATE_OK);
		}
		else if (strcmp ("editor", drawnPrompt->buffer) == 0) {
			setMode (MODE_EDITOR);
		}
		else if (strcmp ("konsole", drawnPrompt->buffer) == 0) {
			//nice (-3);
			parentProcessRuncmd ("konsole &");
			//nice (prognice);
		}
		else if (strcmp ("mainmenu", drawnPrompt->buffer) == 0) {
			setMode (MODE_MAINMENU);
		}
		else if (strcmp ("memfill", drawnPrompt->buffer) == 0) {
			nice (2);
			//system ("./nexus.x -f memfill &");
			system ("./nexus.x --memfill &");
			nice (program_nice);
		}
	//	else if (strcmp ("memfill usb", drawnPrompt->buffer) == 0) {
	//		memfill (MEMFILL_TYPE_DISK, "/mnt/sda1");
	//	}
		else if (strcmp ("memory", drawnPrompt->buffer) == 0) {
			setMode (MODE_MEM);
		}
	//	else if (strcmp ("parent", drawnPrompt->buffer) == 0) {
	//		sprintf (drawnPrompt->text, "parent: Enter a command:");
	//		drawnPrompt->drawn = 1;
	//		
	//	}
		else if (strcmp ("tester", drawnPrompt->buffer) == 0) {
			startTester ();
		}
		else {
			autoclearwait = 2;
			sprintf (msgbuf2, 
					 "%s: no command found, here they are:", drawnPrompt->buffer);
			addMsg (0, "devel: open all .c and .h files with the kate editor");
			addMsg (0, "editor: Switch to mesh editing mode");
			addMsg (0, "konsole: start KDE konsole");
			addMsg (0, "mainmenu: Go to the main menu");
			addMsg (0, "memory: Switch to memory monitoring mode");
			//addMsg (0, "parent: Parent process will execute given command using 'system()'");
			//addMsg (0, "memfill ram: Free 'unused' RAM usage by over-filling it");
			//addMsg (0, "memfill usb: Clear all unused bytes on the media");
			addMsg (0, "rebuild: recompile and restart Nexus");
			autoclearwait += 2;
		}
	}
	else sprintf (drawnPrompt->buffer, "%s%c", drawnPrompt->buffer, key);
}*/

/* prepare for 2d text rendering */
void setOrthographicProjection (void) {
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();
	gluOrtho2D (0.0, (double)winWidth, 0.0, (double)winHeight);
	glScalef (1.0,-1.0, 1.0);
	glTranslatef (0.0, -winHeight, -0.5);
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix ();
	glPushMatrix ();
	glLoadIdentity ();
	// now all hud2dText() calls from the calling function
}

/* restore the 3d rendering */
void setPerspectiveProjection (void) {
	glPopMatrix ();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glMatrixMode (GL_MODELVIEW);
}

void hudTextSetOnceColor (float red, float green, float blue, float alpha) {
	hudTextOnceColor[0] = red;
	hudTextOnceColor[1] = green;
	hudTextOnceColor[2] = blue;
	hudTextOnceColor[3] = alpha;
	hudTextOnceColorFlag = 1;
}

void hudTextSetColor (float red, float green, float blue, float alpha) {
	hudTextColor[0] = red;
	hudTextColor[1] = green;
	hudTextColor[2] = blue;
	hudTextColor[3] = alpha;
}

void hud2dText (int x, int y, char *text2) {
	if (!rendering) {
		if (hudTextOnceColorFlag) hudTextOnceColorFlag = 0;
		return;
	}
	int xx, x2 = x;
	char c;
	//int checkSpace = 0;
	char *tmpstr = malloc (strlen(" >>"));
	sprintf (tmpstr, "%s", " >>");
	
	if (hudTextOnceColorFlag) 
		glColor4fv (hudTextOnceColor);
	//else 
	//	glColor4fv (hudTextColor);
	//if (once64) printf ("hud2dText(): at "); fflush (NULL);
	int ty;
	for (xx = 0;  xx < strlen (text2);  xx++) {
		glRasterPos3f (x2, y, 0.1);
		c = text2[xx];
		glutBitmapCharacter (HUD_FONT, c);
		ty = glutBitmapWidth (HUD_FONT, c) + 2;
		x2 += ty;
		//if (once64) printf ("%d+%d:%c:, ",x,ty,c); fflush (NULL);
		if (drawnPrompt->drawn) drawnPrompt->cursorPosition += glutBitmapWidth (HUD_FONT, c) + 2;
	}
	if (hudTextOnceColorFlag) hudTextOnceColorFlag = 0;
	//if (once64)
		//printf ("::%s::\n", text2);
	free (tmpstr);
}

void hud3dText (char *text2, float x, float y, float z) {
	if (!rendering) {
		if (hudTextOnceColorFlag) hudTextOnceColorFlag = 0;
		return;
	}
	int x2;
	char c;
	glPushMatrix ();
	glRasterPos3f (x,y,z);
	//glRasterPos2f (0.0,2.0);
	if (hudTextOnceColorFlag) {
		glColor4fv (hudTextOnceColor);
	}
	//else {
	//	glColor4fv (hudTextColor);
	//}
	for (x2 = 0;  x2 < strlen (text2);  x2++) {
		c = text2[x2];
		glutBitmapCharacter (HUD_FONT, c);
	}
	if (hudTextOnceColorFlag) hudTextOnceColorFlag = 0;
	glPopMatrix ();
}

void hudUpdateClock (void) {
	hudClockTime = time (NULL);
	//printf ("hudUpdateClock(): @ %llu\n", (unsigned long long int)hudClockTime);
	mtm = localtime (&hudClockTime);
	
	memset (hudClockText, ' ', 79);
	switch (options.showClock) {
		case 0:
			break;
		case 1:
			sprintf (hudClockText, "%s %2.d %s %2.d:%2.d:%2.d", 
				wday[mtm->tm_wday],
				mtm->tm_mday,
				mday[mtm->tm_mon],
				mtm->tm_hour,
				mtm->tm_min,
				mtm->tm_sec);
			break;
		case 2:
			sysinfo (&sinfo);
			sprintf (hudClockText, "%s %2.d %s %2.d:%2.d:%2.d, %2.lu:%2.lu:%2.lu uptime", 
				wday[mtm->tm_wday],
				mtm->tm_mday,
				mday[mtm->tm_mon],
				mtm->tm_hour,
				mtm->tm_min,
				mtm->tm_sec,
				((unsigned long)sinfo.uptime/60)/60,
				((unsigned long)sinfo.uptime/60)%60,
				(unsigned long)sinfo.uptime%60);
			break;
		case 3:
			sysinfo (&sinfo);
			sprintf (hudClockText, "%s %2.d %s %2.d:%2.d:%2.d, %2.lu:%2.lu:%2.lu uptime, %0.2f ticks", 
				wday[mtm->tm_wday],
				mtm->tm_mday,
				mday[mtm->tm_mon],
				mtm->tm_hour,
				mtm->tm_min,
				mtm->tm_sec,
				((unsigned long)sinfo.uptime/60)/60,
				((unsigned long)sinfo.uptime/60)%60,
				(unsigned long)sinfo.uptime%60,
				(double)clock()/1000000);
			break;
		case 4:
			sprintf (hudClockText, "%s %2.d %s %2.d:%2.d:%2.d", 
					wday[mtm->tm_wday],
					mtm->tm_mday,
					mday[mtm->tm_mon],
					mtm->tm_hour,
					mtm->tm_min,
					mtm->tm_sec);
			break;
		case 5:
			sysinfo (&sinfo);
			sprintf (hudClockText, "%2.lu:%2.lu:%2.lu uptime",
					((unsigned long)sinfo.uptime/60)/60,
					((unsigned long)sinfo.uptime/60)%60,
					(unsigned long)sinfo.uptime%60);
			break;
		case 6:
			sprintf (hudClockText, "%0.2f ticks", (double)clock()/1000000);
			break;
		default:
			break;
	}
}

void hudUpdateFPS (void) {
	sprintf (fpsText, "%d FPS", fps);
}

void NLdrawHud (void) {
	glPushMatrix ();
	glTranslatef (winWidth - 100.0, winHeight - 100.0, 0.0);
	glRotatef (delta, 0,1,0);
	glColor4f (0.2,0.6,0.3, 1.0);
	glutWireSphere (10.0, 32, 32);
	glPopMatrix ();
	
	if (msgPending && !msgPauseDrawing)
		drawMsg ();
	//if (mainMenuList.rootMenu->drawn) 
	drawMenu ();
	//if (renderingBrowser) drawBrowser ();
	if (options.showClock)
		drawClock ();
	if (options.showFPS)
		drawFPS ();
	if (options.showSensors)
		drawSensors ();
	if (selecting)
		drawSelectBox ();
	if (drawnPrompt->drawn)
		drawPrompt ();
}

void drawMsg (void) {
	curmsg = &firstmsg;
	char *tmpstr = malloc (strlen(" >>")+1);//, *ts2;
	sprintf (tmpstr, "%s", " >>");
	char *text2;
	linecnt = 0;
	glPushMatrix ();
	glTranslatef (ENTRY_SIZEX + 10.0, 2.0, -0.9);
	glColor4fv (hudMsgTextColor);
	for (;;) { // All messages
		glTranslatef (0.0, 10.0, 0.0);
		text2 = curmsg->text;
		glColor4fv (hudMsgTextColor);
		hud2dText (0.0, 0.0, curmsg->text);
		if (curmsg->next == NULL)
			break;
		curmsg = curmsg->next;
		++linecnt;
		if (options.cutMessages > 0 && linecnt >= options.cutMessages) {
			sprintf (msgbuf2, "+%d msgs", msgPending-15);
			hudTextSetOnceColor (0.7,0.5,0.5, 0.4);
			hud2dText (0.0, 10.0, msgbuf2);
			break;
		}
	}
	glPopMatrix ();
	free (tmpstr);
}
/*
void drawPrompt (void) {
	// Input buffer area background
	glPushMatrix ();
	glTranslatef (10.0, winHeight - 30, 0.9);
		glColor4f (0.2, 0.2, 0.2, hudAlpha);
		glBegin (GL_POLYGON);
		 glVertex3f (0.0, 				0.0,  0.0);
		 glVertex3f (0.0, 				drawnPrompt->sizeY, 0.0);
		 glVertex3f (drawnPrompt->sizeX, drawnPrompt->sizeY, 0.0);
		 glVertex3f (drawnPrompt->sizeX, 0.0, 0.0);
		glEnd ();
		drawingPrompt = 1;
		glColor4fv (hudTextColor);
		hud2dText (2.0, 14.0, drawnPrompt->buffer);
		drawingPrompt = 0;
		
		// Text area background
		glTranslatef (0.0, -drawnPrompt->sizeY, 0.0);
		glColor4f (0.15, 0.15, 0.15, hudAlpha);
		glBegin (GL_POLYGON);
		 glVertex3f (0.0, 				0.0,  0.0);
		 glVertex3f (0.0, 				drawnPrompt->sizeY, 0.0);
		 glVertex3f (drawnPrompt->sizeX, drawnPrompt->sizeY, 0.0);
		 glVertex3f (drawnPrompt->sizeX, 0.0, 0.0);
		glEnd ();
		drawingPrompt = 1;
		glColor4fv (hudTextColor);
		hud2dText (2.0, 14.0, drawnPrompt->text);
		drawingPrompt = 0;
		
		// Blinking cursor
		glBegin (GL_LINE);
		glColor4f (0.7, 0.7, 0.7, hudAlpha);
		glVertex3f (drawnPrompt->cursorPosition, 0.0, 0.0);
		glVertex3f (drawnPrompt->cursorPosition, 10.0, 0.0);
		glEnd ();
	glPopMatrix ();
	
	drawnPrompt->cursorPosition = 0; // because h2dText() does += on it, reset it once done.
}
*/
int messageBox (int type, char *text) {
//	messageBoxPending = 1;
//	sprintf (mb.text, "%s", text);
	return 0;
}

void drawMessageBox (void) {
	if (once64) printf ("drawMessageBox()\n");
	glPushMatrix ();
	glTranslatef (0.0,0.0,-0.4);
	glBegin (GL_POLYGON);
	glColor4f (0.3, 0.4, 0.3, 0.7);
	glVertex3f (winWidth/2 - 100.0, winHeight/2 + 100.0, 0.0);
	glVertex3f (winWidth/2 + 100.0, winHeight/2 - 100.0, 0.0);
	glVertex3f (winWidth/2 + 100.0, winHeight/2 - 100.0, 0.0);
	glVertex3f (winWidth/2 - 100.0, winHeight/2 + 100.0, 0.0);
	glEnd ();
	glPopMatrix ();
	return;
}

/* Time shown in the lower-right corner of the screen, called from drawHud () */
void drawClock (void) {
	glColor4fv (hudTextColor);
	hud2dText (winWidth - 375, winHeight - 4, hudClockText);
}

void drawFPS (void) {
	// show the FPS in the top-right corner
	glColor4fv (hudTextColor);
	//hud2dText (winWidth - (strlen (fpsText) * 12)+2, 10, fpsText);
	hud2dText (0, 20/*winHeight-4*/, fpsText);
}

int gap = 0;
void drawSensors (void) {
	glColor4fv (hudTextColor);
	gap = 0;
	if (options.showFPS) gap = strlen (fpsText)*2;
	hud2dText (gap+65, winHeight-4, CPUtempstr);
}

/*struct sHudObject_EntryList {
	float x, y, z;	// z for alpha and depth channels
	Color color;	// particular user settings for distinctions
	char *label;
	struct sHudObject_Entry *rootEntry, *lastEntry;
};*/
struct sHudObject_EntryList *hudNewEntryList (char *label,
		float x, float y, float z,
		float red, float green, float blue, float alpha) {
	//printf ("hudNewEntryList(): started\n");
	struct sHudObject_EntryList *list = malloc (sizeof (struct sHudObject_EntryList));
	
	typeSetColor (&list->color, red, green, blue, alpha);
	list->label = malloc (strlen (label));
	sprintf (list->label, "%s", label);
	list->rootEntry = NULL;
	list->lastEntry = NULL;
	//printf ("hudNewEntryList(): ended\n");
	return list;
}

void hudShowShortcuts (void) {
	// messageBox (text);
	addMsg (0, "0: Numpad in mode camera or selection");
	//addMsg (0, "Alt + c: Numpad in mode camera");
	addMsg (0, "+ & -: Toggle camera or selection moving step");
	//addMsg (0, "a: Toggle autoclear");
	addMsg (0, "a: use alpha color channel");
	addMsg (0, "C: Clear message board");
	addMsg (0, "c: mode codeParse");
	addMsg (0, "i: show input prompt");
	addMsg (0, "e: mode Mesh editor");
	addMsg (0, "m: mode Main menu or memory monitor");
	addMsg (0, "p: Camera or selection position");
	addMsg (0, "F1: Show shortcuts");
	addMsg (0, "F4: Exit Nexus");
	autoclearwait += 2;
}
