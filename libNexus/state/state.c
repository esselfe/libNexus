#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <browser.h>
#include <camera.h>
#include <drawing.h>
#include <hud.h>
#include <menu.h>
#include <state.h>

int mode = MODE_MAINMENU;
int numpadMode = MODE_CAMERA;
int stateStrSize = 64;

const char *modeStringByModeID (int modeID) {
	switch (modeID) {
	case MODE_IDLE:		return "MODE_IDLE";	break;
	case MODE_MAINMENU: return "MODE_MAINMENU"; break;
	default: break;
	}
	return NULL;
}

void NLsetMode (int newMode) { 
	struct Entry *entry = NULL;
	storeCameraPosition (cam);
	switch (mode) {
		case MODE_BROWSER:
			renderingBrowser = 0;
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_BROWSER);
			break;
		case MODE_BUFFER:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_BUFFER);
			break;
		case MODE_CODEPARSE:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_CODEPARSE);
			break;
		case MODE_EDITOR:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_EDITOR);
			break;
		case MODE_GAME:
//			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_GAME);
			break;
		case MODE_IDLE:
			break;
		case MODE_MAINMENU:
			break;
		case MODE_MEM:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_MEM);
			break;
		case MODE_RACE:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_RACE);
			break;
		default:
			printf ("setMode(): previous mode is unknown!\n");
			break;
	}
	if (entry != NULL) {
		entry->enabled = 0;
		entry = NULL;
	}
	
	switch (newMode) {
		case MODE_BROWSER:
			renderingBrowser = 1;
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_BROWSER);
			numpadMode = MODE_CAMERA;
			renderingModeFunc = renderBrowser;
			break;
		case MODE_BUFFER:
			//entry = entryByID2 (__FILE__,__LINE__,"parentFuncnamehere", ENTRY_MODE_BUFFER);
			//numpadMode = MODE_CAMERA;
			break;
		case MODE_CODEPARSE:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_CODEPARSE);
			numpadMode = MODE_CAMERA;
			renderingModeFunc = renderCodeParse;
			break;
		case MODE_EDITOR:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_EDITOR);
			numpadMode = MODE_SELECTION;
			renderingModeFunc = renderEditor;
			break;
		case MODE_GAME:
			renderingModeFunc = renderGame;
			break;
		case MODE_IDLE:
			break;
		case MODE_MAINMENU:
			numpadMode = MODE_CAMERA;
			renderingModeFunc = renderMainmenu;
			break;
		case MODE_MEM:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_MEM);
			numpadMode = MODE_CAMERA;
			renderingModeFunc = renderMem;
			break;
		case MODE_RACE:
			entry = entryByID2 (__FILE__,__LINE__,"parentfuncnamehere", ENTRY_MODE_RACE);
			numpadMode = MODE_CAMERA;
			renderingModeFunc = renderRace;
			break;
		default:
			printf ("setMode(): new mode is unknown!\n");
			break;
	}
	if (entry != NULL)
		entry->enabled = 1;
	mode = newMode;
	restoreCameraPosition (cam);
	needRedraw = 1;
}

void NLsetState (int newState, int newCode) {
	state.stateNum = newState;
	state.codeNum = newCode;
	switch (newState) {
		case STATE_IDLE:
			sprintf (state.stateStr, "%s", STATE_STR_IDLE);
			break;
		case STATE_ROOTCHK:
			sprintf (state.stateStr, "%s", STATE_STR_ROOTCHK);
			break;
		case STATE_CONFIG:
			sprintf (state.stateStr, "%s", STATE_STR_CONFIG);
			break;
		case STATE_ARGS:
			sprintf (state.stateStr, "%s", STATE_STR_ARGS);
			break;
		case STATE_XINIT:
			sprintf (state.stateStr, "%s", STATE_STR_XINIT);
			break;
		case STATE_GLUTINIT:
			sprintf (state.stateStr, "%s", STATE_STR_GLUTINIT);
			break;
		case STATE_ENGINEINIT:
			sprintf (state.stateStr, "%s", STATE_STR_ENGINEINIT);
			break;
		case STATE_TESTER:
			sprintf (state.stateStr, "%s", STATE_STR_TESTER);
			break;
		case STATE_FUNCTION:
			sprintf (state.stateStr, "%s", STATE_STR_FUNCTION);
			break;
		case STATE_EVENT:
			sprintf (state.stateStr, "%s", STATE_STR_EVENT);
			break;
		case STATE_SIGNAL:
			sprintf (state.stateStr, "%s", STATE_STR_SIGNAL);
			break;
		default:
			printf ("nexus:setState(): unknown newState\n");
			state.stateNum = STATE_UNKNOWN;
			sprintf (state.stateStr, "%s", STATE_STR_UNKNOWN);
			break;
	}
	switch (newCode) {
		case STATE_OK:
			sprintf (state.codeStr, "%s", STATE_STR_OK);
			break;
		case STATE_ERROR:
			sprintf (state.codeStr, "%s", STATE_STR_ERROR);
			break;
		case STATE_WAITING:
			sprintf (state.codeStr, "%s", STATE_STR_WAITING);
			break;
		default:
			printf ("nexus:setState(): unknown newCode\n");
			state.stateNum = STATE_UNKNOWN;
			sprintf (state.stateStr, "%s", STATE_STR_UNKNOWN);
			break;
	}
}

void NLshowState (void) {
	printf ("state: %s %s\n", state.stateStr, state.codeStr);
}

