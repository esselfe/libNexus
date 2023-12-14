#ifndef CONFIG_H
#define CONFIG_H 1
#include <Ntype.h>

extern char *configFileName;

extern int bellVol;
extern int bellPitch;
extern char *xsetcmd;	// Currently used in testerThreadFunc()
extern const int XSETCMDSIZE;

struct OptId {
	int id;
};

struct Opt {
	int id;
	char *name;
	struct Opt *prev, *next;
};
extern struct Opt *optTree;

/* Main program options */
/* Respective in the default config file */
typedef struct {
	int autoclear;	// To remove one message at its value (mseconds) from the GUI message board.
	int cutMessages;
	mbool autoHideMenu;
	mbool autoSaveState;
	int bellVol;	// Used currently by tester(), using system("xset b %d %d %d");
	int bellPitch;
	int bellDuration;
	int niceLevel;
	mbool showClock;
	mbool showFPS;
	mbool showTermFPS;
	mbool showFloor;
	mbool showFloorLines;
	mbool showUsb;
	mbool showSensors;
	int updateDelay;
	mbool useAlpha;
	mbool debug;
	mbool verbose;		// Verbose level from 0 to 4
} Options;
extern Options options;

/* Respective in the config file */
typedef struct {
	mbool showids;
	mbool showPosition;
	mbool showPoint;
	mbool showMeshLines;
	mbool browser;
	mbool buffer;
	mbool displayList;
	mbool drawing;
	mbool events;
	mbool mesh;
	mbool prompt;
} Debug;
extern Debug debug;

void NLinitConfig (void);
void NLloadConfig (void);
void NLsaveConfig (void);

#endif /* CONFIG_H */
