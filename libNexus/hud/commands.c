#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "config.h"
#include "menu.h"
#include "messages.h"
#include "libNexus.h"
#include "Ninit.h"
#include "state.h"
#include "tester.h"

struct Prompt prompt, *drawnPrompt;
int drawingPrompt = 0;

void initCommands (void) {
	prompt.drawn = 0;
	prompt.text = malloc (MESSAGESIZE);
	prompt.buffer = malloc (MESSAGESIZE);
	memset (prompt.text, 0, MESSAGESIZE);
	memset (prompt.buffer, 0, MESSAGESIZE);
	prompt.cursorPosition = 2.0;
	prompt.x = 5.0;
	prompt.y = winHeight - 20.0;
	prompt.sizeX = winWidth - ENTRY_SIZEX -10.0;
	prompt.sizeY = 20.0;
	drawnPrompt = &prompt;
}

char *prompt2 (char *text) {
	memset (drawnPrompt->text, 0, MESSAGESIZE);
	sprintf (drawnPrompt->text, text);
	
	memset (drawnPrompt->buffer, 0, MESSAGESIZE);
	drawnPrompt->cursorPosition = 2.0;
	drawnPrompt->drawn = 1;

	return "nothing";
}

void parsePrompt (unsigned char key) {
//	if (debug.prompt) printf ("parsePrompt(): key :%c: %d\n", key, key);
	if (key == 8) {		// Backspace
		char *cc = drawnPrompt->buffer + strlen (drawnPrompt->buffer) -1;
		*cc = '\0';
		return;
	}
	if (key == 13) {	// Enter
		drawnPrompt->drawn = 0;
		

		if (strcmp (drawnPrompt->buffer, "qw") == 0)
			exitNexus (STATE_EVENT, STATE_OK, __FUNCTION__);
		else if (strcmp (drawnPrompt->buffer, "editor") == 0)
			NLsetMode (MODE_EDITOR);
		else if (strcmp (drawnPrompt->buffer, "main menu") == 0)
			NLsetMode (MODE_MAINMENU);
		else if (strcmp (drawnPrompt->buffer, "memfill") == 0)
			system ("memfiller --useXterm 2>/dev/null");
		else if (strcmp (drawnPrompt->buffer, "memory") == 0)
			NLsetMode (MODE_MEM);
		else if (strcmp (drawnPrompt->buffer, "tester") == 0)
			startTester ();
		else {
			autoclearwait = 2;
			sprintf (msgbuf2, "%s: No command found", drawnPrompt->buffer);
			addMsg (0, msgbuf2);
			addMsg (0, "Few commands: editor, main menu, memfill, memory, tester");
		}
	}
	else sprintf (drawnPrompt->buffer, "%s%c", drawnPrompt->buffer, key);

}

