#ifndef STATE_H
#define STATE_H 1

#include "camera.h"

// ***************
// Internal state and/or state flags, thought for debugging purposes
// ***************
struct {
	int stateNum;
	int codeNum;
	char *stateStr;
	char *codeStr;
} state;

// States
#define STATE_IDLE			0x00
#define STATE_ROOTCHK		0x01
#define STATE_CONFIG		0x02
#define STATE_ARGS			0x03
#define STATE_XINIT			0x04
#define STATE_GLUTINIT		0x05
#define STATE_ENGINEINIT	0x06
#define STATE_TESTER		0x07
#define STATE_FUNCTION		0x08

#define STATE_EVENT			0x80	// The GUI menu entry "exit" has been clicked

#define STATE_SIGNAL		0x100	// That is for the signal handler function

// State codes
#define STATE_OK			0xFF
#define STATE_ERROR			0xFE
#define STATE_WAITING		0xFD

#define STATE_UNKNOWN		0x80

extern int stateStrSize;

// used in showState() and setState()
#define STATE_STR_IDLE			"idle"
#define STATE_STR_ROOTCHK		"rootchk"
#define STATE_STR_CONFIG 		"config"
#define STATE_STR_ARGS			"args"
#define STATE_STR_XINIT			"xinit"
#define STATE_STR_GLUTINIT		"glutInit"
#define STATE_STR_ENGINEINIT	"engineInit"
#define STATE_STR_TESTER		"tester"
#define STATE_STR_FUNCTION		"function"
#define STATE_STR_EVENT			"event"
#define STATE_STR_SIGNAL		"signal"

#define STATE_STR_OK		"ok"
#define STATE_STR_ERROR		"error"
#define STATE_STR_WAITING	"waiting"

#define STATE_STR_UNKNOWN	"unknown"




// ***********************
// User mode program state
// ***********************
//
typedef struct Mode {
	int id;
	char *name;
} sMode;

typedef struct ModeList {
	struct Mode rootMode, *lastMode;
} sModeList;

extern int mode;

#define MODE_IDLE 			0xFF
#define MODE_MAINMENU 		1
#define MODE_BROWSER		2
#define MODE_MEM 			3
#define MODE_CPU 			4
#define MODE_GAME			5
#define MODE_EDITOR			6
#define MODE_CODEPARSE		7
#define MODE_RACE			8
#define MODE_BUFFER			9

extern int numpadMode;
#define MODE_CAMERA 		100
#define MODE_SELECTION		101

const char *modeStringByModeID (int modeID);
void NLsetMode (int newMode);
void NLsetState (int newState, int newCode);
void NLshowState (void);
#endif	/* STATE_H */

