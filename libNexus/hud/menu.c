#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glut.h>

#include "Ntype.h"
#include "config.h"
#include "drawing.h"
#include "messages.h"
#include "libNexus.h"
#include "events.h"
#include "hud.h"
#include "menu.h"
#include "util.h"

int ENTRY_SIZEX = 100;
int ENTRY_SIZEY = 20;
int SUBENTRY_SIZEX = 125;
int ENTRY_EDITOR_SIZEX = 100;
int SUBENTRY_EDITOR_SIZEX = 125;

//int menuState;
float defaultEntryColor[4] = {0.05,0.1,0.05, 0.75};
int maxEntrySizeX = 0;
int maxSubentrySizeX = 0;
int maxEntryEditorSizeX = 0;
int maxSubentryEditorSizeX = 0;
int stateInSubmenu = 0;

void initMenu2 (void) {
	printf ("initMenu2(): started\n");
	printf ("initMenu2(): done\n");
}

void initMenu (void) {
	/*menu.x = 0.0;
	menu.y = (float)winHeight - 20.0;
	menu.drawn = 1;
	menu.rootEntry = NULL;
	menu.lastEntry = NULL;*/
	// Build the program menu
	/////////////////////////
	mainMenuList.rootMenu = NULL;
	mainMenuList.lastMenu = NULL;
	sMenu *menu = addMenu ("Program menu", 2.0, (float)winHeight - 16.0);
	drawnMenu = menu;
	
	struct Entry *entry, *subentry;
	
	addEntry (menu, ENTRY_TYPE_ACTION, ENTRY_EXIT, "Exit");
	entry = addEntry (menu, ENTRY_TYPE_SUBMENU, ENTRY_SUBMENU_OPTIONS, "Options");
	addSubentry (entry,
				 ENTRY_TYPE_ACTION, ENTRY_OPTIONS_SHORTCUTS, "Shortcuts");	
	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_AUTOCLEAR, "Autoclear");
	if (options.autoclear)
		subentry->enabled = 1;

	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_AUTOSAVESTATE, "AutoSaveState");
	if (options.autoSaveState) 
		subentry->enabled = 1;

	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_CLEARMSG, "Clear msg");
	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_SHOWCLOCK, "Show clock");
	if (options.showClock) 
		subentry->enabled = 1;
	
	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_SHOWFPS, "Show FPS");
	if (options.showFPS) 
		subentry->enabled = 1;
	
	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_SHOWTERMFPS, "Show termFPS");
	if (options.showTermFPS)
		subentry->enabled = 1;
	
	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_SHOWFLOOR, "Show floor");
	if (options.showFloor) 
		subentry->enabled = 1;
	

	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_DEBUG, "Debug");
	//if (options.debug) {
	//	subentry->enabled = 1;
	
	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_SHOWSENSORS, "Show sensors");
	if (options.showSensors)
		subentry->enabled = 1;
	
	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_USEALPHA, "Use alpha");
	if (options.useAlpha)
		subentry->enabled = 1;
	

	subentry = addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_DEBUG_BUFFER, "Buffer");
	if (debug.buffer)
		subentry->enabled = 1;

//		entry = addSubentry (entry, ENTRY_TYPE_SUBMENU, ENTRY_SUBMENU_VERBOSE, "Verbose");
//			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_VERBOSE_L0, "Off");
//			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_VERBOSE_L1, "Level 1");
//			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_VERBOSE_L2, "Level 2");
//			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_VERBOSE_L3, "Level 3");
//			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_OPTIONS_VERBOSE_L4, "Level 4");
//			entry = entryByID (ENTRY_SUBMENU_OPTIONS);
// 		entry = addSubentry (entry, ENTRY_TYPE_SUBMENU, ENTRY_SUBMENU_FONT, "HUD Font");
// 			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_FONT_9x15, "9x15");
// 			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_FONT_8x13, "8x13");
// 			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_FONT_TIMES_ROMAN_10, "Times roman 10");
// 			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_FONT_TIMES_ROMAN_24, "Times roman 24");
// 			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_FONT_HELVETICA_10, "Helvetica 10");
// 			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_FONT_HELVETICA_12, "Helvetica 12");
// 			addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_FONT_HELVETICA_18, "Helvetica 18");
	entry = addEntry (menu, ENTRY_TYPE_SUBMENU, ENTRY_SUBMENU_MODE, "Mode");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_MODE_EDITOR, "Editor");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_MODE_MEM, "Memory");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_MODE_CODEPARSE, "Code parse");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_MODE_BROWSER, "Browser");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_MODE_RACE, "Race");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_MODE_BUFFER, "Buffer");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_MODE_GAME, "Game");
	
	//struct Menu *editMenu = addMenu ("editMenu");
	//addEntry (editMenu, ENTRY_TYPE_ACTION, ENTRY_EDITOR_MAINMENU, "Main menu");
	//addEntry (editMenu, ENTRY_TYPE_SUBMENU, ENTRY_EDITOR_SUBMENU_ADD, "Add");
	//entry = entryByID (ENTRY_EDITOR_SUBMENU_ADD);
	//addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_ADD_POINT, "Point");
	//addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_ADD_PLANE, "Plane");
	//addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_ADD_MESH, "Mesh");
	//addEntry (editMenu, ENTRY_TYPE_SUBMENU, ENTRY_EDITOR_SUBMENU_FILE, "File");
	//entry = entryByID (ENTRY_EDITOR_SUBMENU_FILE);
	//addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_FILE_SAVE, "Save mesh");
	//addSubentry (entry, ENTRY_TYPE_SUBMENU, ENTRY_EDITOR_FILE_LOAD, "Load mesh");
	//entry = entryByID (ENTRY_EDITOR_FILE_LOAD);
	//addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_FILE_LOAD_DEFAULTMESH, "DefaultMesh");
	//addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_FILE_LOAD_ARMYTRUCK, "ArmyTruck");
	entry = addEntry (menu, ENTRY_TYPE_SUBMENU, ENTRY_EDITOR_SUBMENU_DEBUG, "Debug");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_DEBUG_SHOWIDS, "mesh point ids");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_DEBUG_SHOWPOSITION, "positions");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_DEBUG_SHOWPOINT, "mesh points");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_DEBUG_SHOWLINES, "mesh lines");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_DEBUG_DRAWING, "Drawing");
	//destroyEntry (ENTRY_SUBMENU_OPTIONS);
	
	// Build the meshEditing menu
	/////////////////////////////
	
/*	menu = addMenu ("Mesh editing menu", 2.0, (float)winHeight - 120.0, ENTRY_SIZEX, ENTRY_SIZEY);
	entry = addEntry (menu, ENTRY_TYPE_SUBMENU, ENTRY_EDITOR_SUBMENU_FILE, "File");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_FILE_SAVE, "Save mesh");
	addSubentry (entry, ENTRY_TYPE_ACTION, ENTRY_EDITOR_FILE_LOAD, "Load mesh");
*/}

sMenu *addMenu (char *label, float x, float y) {
	sMenu *menu;
	if (mainMenuList.rootMenu == NULL) {
		mainMenuList.rootMenu = malloc (sizeof (sMenu));
		menu = mainMenuList.rootMenu;
		menu->prev = NULL;
		menu->next = NULL;
		//menu->y = y;
	}
	else {
		menu = mainMenuList.lastMenu;
		menu->next = malloc (sizeof (sMenu));
		menu->next->prev = menu;
		menu = menu->next;
		menu->next = NULL;
		//menu->y = winHeight - y+1.0;
	}
	menu->rootEntry = NULL;
	menu->lastEntry = NULL;
	menu->y = y;
	menu->x = x;
	menu->width = ENTRY_SIZEX;
	menu->height = ENTRY_SIZEY;
	menu->drawn = 1;
	menu->label = malloc (strlen (label));
	sprintf (menu->label, "%s", label);
	mainMenuList.lastMenu = menu;
	return menu;
}

struct Entry *addEntry (sMenu *menu, int type, int ID, char *label){
	//printf ("addEntry(): started :%s:\n", label);
	struct Entry *entry;
	if (menu->rootEntry == NULL) {
		menu->rootEntry = malloc (sizeof (struct Entry));
		entry = menu->rootEntry;
		entry->prev = NULL;
		entry->rank = 1;
		entry->y = 0.0;
	}
	else {
		entry = menu->lastEntry;
		entry->next  = malloc (sizeof (struct Entry));
		entry->next->prev = entry;
		entry = entry->next;
		entry->rank = entry->prev->rank + 1;
		entry->y = entry->prev->y - ENTRY_SIZEY-1.0;
	}
	entry->selectID = ID;//menu->ID + 100;
	entry->type = type;
	entry->drawn = 0;
	entry->enabled = 0;
	entry->label = malloc (ENTRY_LABEL_LENGHT);
	 memset (entry->label, 0, ENTRY_LABEL_LENGHT-1);
	 sprintf (entry->label, "%s", label);
	entry->x = menu->x;
	entry->color[0] = defaultEntryColor[0];
	entry->color[1] = defaultEntryColor[1];
	entry->color[2] = defaultEntryColor[2];
	entry->color[3] = defaultEntryColor[3];
	entry->next = NULL;
	entry->submenu = NULL;
	entry->subentry = NULL;
	entry->lastSubentry = NULL;
	
	menu->height += (float)ENTRY_SIZEY;
	menu->lastEntry = entry;
	//printf ("addEntry(): added :%s: rank %d ID %d\n", entry->label, entry->rank, entry->selectID);
	return entry;
}

// Adds a subentry to submenu
struct Entry *addSubentry (struct Entry *submenu, int type, int ID, char *label) {
	struct Entry *entry;
	if (submenu->type != ENTRY_TYPE_SUBMENU) {
		sprintf (msgbuf2,
				 "addSubentry(): Not submenu-typed. Type is #%d", submenu->type);
		addMsg (0, msgbuf2);
		return NULL;
	}
	else {
		if (submenu->subentry == NULL) {
			submenu->subentry = malloc (sizeof (struct Entry));
			entry = submenu->subentry;
			entry->prev = NULL;
			entry->rank = 1;
			entry->y = submenu->y;
		}
		else {
			submenu->lastSubentry->next = malloc (sizeof (struct Entry));
			entry = submenu->lastSubentry->next;
			entry->prev = submenu->lastSubentry;
			entry->rank = entry->prev->rank+1;
			entry->y = entry->prev->y - ENTRY_SIZEY - 1.0;
		}
		//printf ("\tdone setting entry\n");
		entry->drawn = 0;
		entry->enabled = 0;
		entry->label = malloc (strlen (label)+1);
		sprintf (entry->label, "%s", label);
		entry->type = type;
		entry->selectID = ID;
		if (submenu->submenu == NULL)
			entry->x = submenu->x + ENTRY_SIZEX + 1.0;
		else
			entry->x = submenu->x + SUBENTRY_SIZEX + 1.0;
		entry->color[0] = defaultEntryColor[0];
		entry->color[1] = defaultEntryColor[1];
		entry->color[2] = defaultEntryColor[2];
		entry->color[3] = defaultEntryColor[3];
		entry->next  = NULL;
		entry->submenu = submenu;
		entry->subentry = NULL;
		entry->lastSubentry = NULL;
		submenu->lastSubentry = entry;
		//printf ("addSubentry(): added :%s: rank %d ID %d\n", entry->label, entry->rank, entry->selectID);
	}
	return submenu->lastSubentry;
}

int destroyEntry (int entryID) {
	printf ("destroyEntry(): started\n");
	struct Entry *entry = entryByID (entryID);
	if (entry == NULL) return 1;
	
	if (entry->prev) {
		if (entry->next) entry->prev->next = entry->next;
		else entry->prev->next = NULL;
	}
	else if (entry->next) {
		entry->next->prev = NULL;
	}
	
	free (entry->label);
	if (entry->subentry) {
		entry = entry->subentry;
		printf ("\tfreeing :%s: %d\n", entry->submenu->label, entry->submenu->selectID);
		free (entry->submenu);
		entry->submenu = NULL;
	}
	else {
		printf ("\tfreeing :%s: %d\n", entry->label, entry->selectID);
		free (entry);
		return 0;
	}
	while (1) {
		if (entry->subentry) {
			entry = entry->subentry;
			continue;
		}
		else {
			free (entry->label);
			if (entry->next) {
				entry = entry->next;
				printf ("\tfreeing :%s: %d\n", entry->prev->label, entry->prev->selectID);
				free (entry->prev);
				continue;
			}
			else if (entry->submenu) {
				entry = entry->submenu;
				printf ("\tfreeing :%s: %d\n", 
						entry->lastSubentry->label, entry->lastSubentry->selectID);
				free (entry->lastSubentry);
			}
			else break;
		}
	}
	
	printf ("removeEntry(): done\n");
	return 0;
}

struct Entry *entryBySelectID (int selectID) {
	return NULL;
}

struct Entry *entryByID2 (char *parentfilename, int line, char *parentfuncname, int ID) {
	struct Entry *entry = entryByID (ID);
	if (entry == NULL) printf ("entryByID2(%d) (called from %s:%d:%s)\n", 
			ID,parentfilename,line,parentfuncname);
	return entry;
}

struct Entry *entryByID (int ID) {
	//printf ("entryByID(): want ID %d\n", ID);
	struct Entry *entry = mainMenuList.rootMenu->rootEntry;
	while (1) {
		//printf ("\tlooking entry :%s: ID %d\n", entry->label, entry->selectID);
		if (entry->selectID == ID) break;
		
		if (entry->subentry != NULL) {
			entry = entry->subentry;
			continue;
		}
		
		if (entry->next == NULL) {
			while (1) {
				if (entry->submenu != NULL) {
					entry = entry->submenu;
					//if (entry->next != NULL) {
					//	entry = entry->next;
						break;
					//}
					
				}
				else break;
			}
		}
		
		if (entry->next != NULL)
			entry = entry->next;
		else break;
	}
	if (entry->selectID != ID) {
		printf ("entryByID(): couldn't find entry ID %d\n", ID);
		return NULL;
	}
	//printf ("entryByID(): return :%s: rank %d ID %d\n", entry->label, entry->rank, entry->selectID);
	return entry;
}

void disableEntry (char *label) {
	return;
}

void hideEntry (char *label) {
	return;
}

void showMenu (int x, int y) {
	mainMenuList.rootMenu->x = x;
	mainMenuList.rootMenu->y = y;
	mainMenuList.rootMenu->drawn = 1;
	needRedraw = 1;
}

/* Toggles 'drawn' for all submenu entries */
void showSubmenu (struct Entry *submenu) {
	if (submenu->subentry == NULL) {
		printf ("showSubmenu(%s): No subentry in this menu\n", submenu->label);
		return;
	}
	struct Entry *entry = submenu->subentry;
	while (1) {
		entry->drawn = entry->drawn ? 0 : 1;
		//printf ("showSubmenu(): ID#%d labeled :%s: = %d\n", entry->selectID, entry->label,  entry->drawn);
		if (entry->next == NULL) break;
		entry = entry->next;
	}
	needRedraw = 1;
}

void hideSubmenu (struct Entry *submenu) {
	return;
}

void menuHideAll (void) {
	//menu.drawn = 0;
	struct Entry *entry = mainMenuList.rootMenu->rootEntry;
	while (1) {
		entry->drawn = 0;
		if (entry->subentry) {
			entry = entry->subentry;
			continue;
		}
		if (entry->next == NULL) {
			while (1) {
				if (entry->submenu != NULL) {
					entry = entry->submenu;
					if (entry->next == NULL) continue;
					else break;
				}
				else break;
			}
		}
		if (entry->next != NULL) entry = entry->next;
		else break;
	}
	needRedraw = 1;
}

void drawEntry (struct Entry *e2) {
		glPushMatrix ();
	glLoadName (e2->selectID);
	glBegin (GL_POLYGON);
		glColor4fv (e2->color);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (0.0, (float)-ENTRY_SIZEY, 0.0);
		glVertex3f ((float)ENTRY_SIZEX, (float)-ENTRY_SIZEY, 0.0);
		glVertex3f ((float)ENTRY_SIZEX, 0.0, 0.0);
	glEnd ();
	glBegin (GL_LINE_LOOP);
		glColor4f (0.0, 0.4, 0.0, 0.75);
		glVertex3f (0.0, 0.0, 0.1);
		glVertex3f (0.0, (float)-ENTRY_SIZEY, 0.1);
		glVertex3f ((float)ENTRY_SIZEX, (float)-ENTRY_SIZEY, 0.1);
		glVertex3f ((float)ENTRY_SIZEX, 0.0, 0.1);
	glEnd ();
	if (e2->type == ENTRY_TYPE_SUBMENU) {
		glBegin (GL_POLYGON);
			glColor3f (0.7,0.8,0.7);
			glVertex3f (ENTRY_SIZEX-5.0, -ENTRY_SIZEY/2+3.0, 0.1);
			glVertex3f (ENTRY_SIZEX-5.0, -ENTRY_SIZEY/2-3.0, 0.1);
			glVertex3f (ENTRY_SIZEX-2.0, -ENTRY_SIZEY/2, 0.1);
		glEnd ();
	}
	if (rendering) {
		glColor4fv (hudTextColor);
		hud2dText (2.0, -ENTRY_SIZEY/2+3.75, e2->label);
	}
	glPopMatrix ();
}

void drawSubentry (struct Entry *curEntry2) {
		glPushMatrix ();
	glLoadName (curEntry2->selectID);
	glBegin (GL_POLYGON);
		glColor4fv (curEntry2->color);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (0.0, (float)-ENTRY_SIZEY, 0.0);
		glVertex3f ((float)SUBENTRY_SIZEX, (float)-ENTRY_SIZEY, 0.0);
		glVertex3f ((float)SUBENTRY_SIZEX, 0.0, 0.0);
	glEnd ();
	glBegin (GL_LINE_LOOP);
		glColor4f (0.0, 0.4, 0.0, 0.75);
		glVertex3f (0.0, 0.0, 0.1);
		glVertex3f (0.0, (float)-ENTRY_SIZEY, 0.1);
		glVertex3f ((float)SUBENTRY_SIZEX, (float)-ENTRY_SIZEY, 0.1);
		glVertex3f ((float)SUBENTRY_SIZEX, 0.0, 0.1);
	glEnd ();
	if (curEntry2->type == ENTRY_TYPE_SUBMENU) {
		glBegin (GL_POLYGON);
			glColor4f (0.7,0.8,0.7, 0.5);
			glVertex3f (SUBENTRY_SIZEX-5.0, -ENTRY_SIZEY/2+3.0, 0.1);
			glVertex3f (SUBENTRY_SIZEX-5.0, -ENTRY_SIZEY/2-3.0, 0.1);
			glVertex3f (SUBENTRY_SIZEX-2.0, -ENTRY_SIZEY/2, 0.1);
		glEnd ();
	}
	else if (curEntry2->enabled == 1) {
		glBegin (GL_POLYGON);
		glColor4f (0.7,0.8,0.7, 0.5);
			glVertex3f (SUBENTRY_SIZEX-5.0, -ENTRY_SIZEY/2+3.0, 0.1);
			glVertex3f (SUBENTRY_SIZEX-5.0, -ENTRY_SIZEY/2-3.0, 0.1);
			glVertex3f (SUBENTRY_SIZEX-2.0, -ENTRY_SIZEY/2-3.0, 0.1);
			glVertex3f (SUBENTRY_SIZEX-2.0, -ENTRY_SIZEY/2+3.0, 0.1);
		glEnd ();
	}
	if (rendering) {
		glColor4fv (hudTextColor);
		hud2dText (4.0, -ENTRY_SIZEY/2+3.75, curEntry2->label);
	}
	glPopMatrix ();
}

void drawMenu3 (void) {
	if (!mainMenuList.rootMenu->drawn) return;
	sMenu *menu = mainMenuList.rootMenu;
	struct Entry *entry = menu->rootEntry;
	//struct Entry *submenu = NULL;
	
	printf ("drawMenu3(): menu->label: %s\n", menu->label);
	while (1) {
		
		if (entry->next == NULL) break;
		else entry = entry->next;
	}
	
}

int level;
void drawMenu (void) {
	if (!mainMenuList.rootMenu->drawn) {
		//glPushMatrix ();
		//glTranslatef 
		glColor4f (0.45, 0.25, 0.25, 1.0);
		glBegin (GL_LINE_STRIP);
		glVertex2f (0.0, winHeight-20.0);
		glVertex2f (4.0, winHeight-20.0);
		glVertex2f (4.0, winHeight- mainMenuList.rootMenu->height);
		glVertex2f (0.0, winHeight- mainMenuList.rootMenu->height);
		glEnd ();
		return;
	}
	glInitNames ();
	glPushName (0);
	
	sMenu *menu = mainMenuList.rootMenu;
	struct Entry *entry = menu->rootEntry;
//	struct Entry *subentry = NULL;
	struct Entry *submenu = NULL;
	
	mbool donesubentries = 0;
	while (1) {	// All menus
		glPushMatrix ();
		glTranslatef (menu->x, menu->y, 0.1);
		if (!menu->drawn) {
			glPushMatrix ();
			//glTranslatef (0.0, winHeight - 10.0, 0.5);
			glColor4f (0.8, 0.5, 0.2, 0.8);
			glBegin (GL_LINE_STRIP);
			 glVertex2f (0.0, 0.0);
			 glVertex2f (5.0, 0.0);
			 glVertex2f (5.0, -menu->height);
			 glVertex2f (0.0, -menu->height);
			glEnd ();
			glPopMatrix ();
			break;
		}
		else {
			while (1) { // All menu entries
				glPushMatrix ();
				glTranslatef (entry->x , entry->y, 0.0);
				if (once64) printf ("\tdrawing entry ::%s menupos:%.2f %.2f entrypos:%.2f %.2f\n", entry->label, menu->x, menu->y, entry->x, entry->y);
				drawEntry (entry);
				glPopMatrix ();
				
				if (entry->subentry != NULL && entry->subentry->drawn == 1) {
					if (once64) printf ("\tdrawing submenu ::%s\n", entry->label);
					submenu = entry;
					entry = entry->subentry;
					level = 1;
					//glPushMatrix ();
					while (!donesubentries) { // Draw all submenu subentries
						if (once64) printf ("\t\tdrawing ::%s\n", entry->label);
						glPushMatrix ();
						glTranslatef (entry->x, entry->y, 0.0);
						drawSubentry (entry);
						glPopMatrix ();
						
						if (entry->subentry != NULL && entry->subentry->drawn == 1) {
							entry = entry->subentry;
							++level;
							continue;
						}
						if (entry->next == NULL) {
							while (1) {
								entry = entry->submenu;
								--level;
								if (entry->submenu == NULL) {
									donesubentries = 1;
									break;
								}
								if (entry->next == NULL) continue;
								else {
									entry = entry->next;
									break;
								}
							}
						}
						else entry = entry->next;
					}
				}
				if (entry->next == NULL) {
					break;
				}
				else entry = entry->next;
			}
		} // ::else if (menu->drawn)
		glPopMatrix ();
		if (menu->next == NULL) break;
		else {
			menu = menu->next;
			entry = menu->rootEntry;
		}
	}
	//glPopMatrix ();
	if (once64) once64 = 0;
}

void drawMenu2 (void) {
	if (!mainMenuList.rootMenu->drawn) return;
	sMenu *menu = mainMenuList.rootMenu;
	struct Entry *entry = menu->rootEntry;
	struct Entry *subentry = NULL;

	glInitNames ();
	glPushName (0);

	glPushMatrix ();
	glTranslatef (menu->x, menu->y, 0.1);
	
	if (once64) printf ("drawMenu2():\n\t :%s:\n", entry->label);
	drawEntry (entry);
	if (entry->next == NULL) return;
	else entry = entry->next;


//	if (once64) printf ("menu at %.2f,%.2f\n", menu->x, menu->y);
	while (1) {
		glTranslatef (0.0, -ENTRY_SIZEY-2.0, 0.0);
		if (once64) printf ("\t :%s:\n", entry->label);
		drawEntry (entry);
		
		if (entry->subentry != NULL && entry->subentry->drawn == 1) {
			subentry = entry->subentry;
			glPushMatrix ();
			glTranslatef (ENTRY_SIZEX, 0.0, 0.0);
			if (once64) printf ("\t\t :%s:\n", subentry->label);
			drawSubentry (subentry);
			if (subentry->next != NULL) {
				subentry = subentry->next;
				while (1) {
					glTranslatef (0.0, -ENTRY_SIZEY-2.0, 0.0);
					if (once64) { printf ("\t\t :%s:\n", subentry->label); sleep (1); }
					drawSubentry (subentry);
					if (subentry->next == NULL) break;
					else subentry = subentry->next;
				}
			}
			glPopMatrix ();
		}

		if (entry->next == NULL) break;
		else entry = entry->next;
	}
	
	
	
	
	glPopMatrix ();
}

