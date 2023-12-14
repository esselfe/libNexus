#ifndef MENU_H
#define MENU_H 1

#include <Ntype.h>

#define ENTRY_EXIT							1	// exit button id

#define ENTRY_SUBMENU_OPTIONS				9
#define ENTRY_SUBMENU_MODE					11
#define ENTRY_SUBMENU_DEBUG					12
//#define ENTRY_SUBMENU_FONT				
//#define ENTRY_SUBMENU_VERBOSE				

#define ENTRY_OPTIONS_SHORTCUTS			40
#define ENTRY_OPTIONS_AUTOCLEAR			41
#define ENTRY_OPTIONS_AUTOSAVESTATE		42
#define ENTRY_OPTIONS_CLEARMSG			43
#define ENTRY_OPTIONS_SHOWCLOCK			44
#define ENTRY_OPTIONS_SHOWFPS			45
#define ENTRY_OPTIONS_SHOWTERMFPS		46
#define ENTRY_OPTIONS_SHOWFLOOR			47
#define ENTRY_OPTIONS_DEBUG				48
#define ENTRY_OPTIONS_SHOWSENSORS		49
#define ENTRY_OPTIONS_USEALPHA			50
/*#define ENTRY_OPTIONS_VERBOSE_L0			48	// General information
#define ENTRY_OPTIONS_VERBOSE_L1			49
#define ENTRY_OPTIONS_VERBOSE_L2			50
#define ENTRY_OPTIONS_VERBOSE_L3			51	// Debug and devel info to console only
#define ENTRY_OPTIONS_VERBOSE_L4			52	// Debug and devel info to console and message board
*/ /* GLUT_BITMAP_9_BY_15
GLUT_BITMAP_8_BY_13
GLUT_BITMAP_TIMES_ROMAN_10
GLUT_BITMAP_TIMES_ROMAN_24
GLUT_BITMAP_HELVETICA_10
GLUT_BITMAP_HELVETICA_12
GLUT_BITMAP_HELVETICA_18 */
#define ENTRY_FONT_9x15					60
#define ENTRY_FONT_8x13					61
#define ENTRY_FONT_TIMES_ROMAN_10		62
#define ENTRY_FONT_TIMES_ROMAN_24		63
#define ENTRY_FONT_HELVETICA_10			64
#define ENTRY_FONT_HELVETICA_12			65
#define ENTRY_FONT_HELVETICA_18			66

//#define ENTRY_MODE_IDLE					70
//#define ENTRY_MODE_MAINMENU				70
#define ENTRY_MODE_EDITOR				71		// for the editor mode
#define ENTRY_MODE_MEM					72
#define ENTRY_MODE_CODEPARSE			73
#define ENTRY_MODE_BROWSER				74
#define ENTRY_MODE_RACE					75
#define ENTRY_MODE_BUFFER				76
#define ENTRY_MODE_GAME					77


#define ENTRY_DEBUG_BUFFER 				100

#define ENTRY_EDITOR_MAINMENU			200
#define ENTRY_EDITOR_HELP_SHORTCUTS		201

#define ENTRY_EDITOR_SUBMENU_ADD		202
#define ENTRY_EDITOR_ADD_POINT			203
//#define ENTRY_EDITOR_ADD_SEGMENT		204
#define ENTRY_EDITOR_ADD_PLANE			205
#define ENTRY_EDITOR_ADD_MESH			206

#define ENTRY_EDITOR_SUBMENU_FILE		207
#define ENTRY_EDITOR_FILE_SAVE			208
#define ENTRY_EDITOR_FILE_LOAD			209
#define ENTRY_EDITOR_FILE_LOAD_DEFAULTMESH		210
#define ENTRY_EDITOR_FILE_LOAD_ARMYTRUCK		211

#define ENTRY_EDITOR_SUBMENU_DEBUG				1000
#define ENTRY_EDITOR_DEBUG_SHOWIDS				1001
#define ENTRY_EDITOR_DEBUG_SHOWPOSITION			1002
#define ENTRY_EDITOR_DEBUG_SHOWPOINT			1003
#define ENTRY_EDITOR_DEBUG_SHOWLINES			1004
#define ENTRY_DEBUG_DRAWING				1005

#define MENU_LABEL_LENGHT				64
#define ENTRY_LABEL_LENGHT				32
extern int ENTRY_SIZEX;
extern int ENTRY_EDITOR_SIZEX;
extern int ENTRY_SIZEY;
extern int SUBENTRY_SIZEX;
extern int SUBENTRY_EDITOR_SIZEX;

extern int maxEntrySizeX;
extern int maxSubentrySizeX;
extern int maxEntryEditorSizeX;
extern int maxSubentryEditorSizeX;
extern int stateInSubmenu;
extern float defaultEntryColor[4];

struct Entry {
	struct Entry *prev, *next, *submenu, *subentry, *lastSubentry;
	int rank;		// Used for drawing positionning
	int selectID;	// Used by the mouse picking function
	int type;		// action or submenu entry?
	mbool drawn;
	mbool enabled;	// mostly for toggleable options
	char *label;
	float x, y;
	float color[4];
} *curEntry, *curEntry2, *curSubentry, *hoveredEntry;

typedef struct Menu {
	struct Menu *prev, *next;
	struct Entry *rootEntry, *lastEntry;
	float x, y, height, width;
	mbool drawn;
	char *label;
} sMenu;

typedef struct MenuList {
	sMenu *rootMenu, *lastMenu;
} sMenuList;
sMenuList mainMenuList;

void initMenu (void);
void initMenu2 (void);
sMenu *addMenu (char *label, float x, float y);
struct Entry *addEntry (sMenu *menu, int type, int ID, char *label);
struct Entry *addSubentry (struct Entry *submenu, int type, int ID, char *label);
int destroyEntry (int entryID);
struct Entry *entryByID2 (char *parentfilename, int line, char *parentfuncname, int ID);
struct Entry *entryByID (int ID);
struct Entry *entryByLabel (char *label);
struct Entry *entryBySelectID (int selectID);
void disableEntry (char *label);
void hideEntry (char *label);
void showMenu (int x, int y);
void showSubmenu (struct Entry *submenu);
void hideSubmenu (struct Entry *submenu);
void menuHideAll (void);
void drawEntry (struct Entry *curEntry2);
void drawSubentry (struct Entry *curEntry2);
void drawMenu3 (void);
void drawMenu (void);
void drawMenu2 (void);

#endif /* MENU_H */
