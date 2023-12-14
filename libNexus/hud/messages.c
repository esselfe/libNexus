#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <drawing.h>
#include <messages.h>

const short MESSAGESIZE = 1024;
char *msgbuf, *msgbuf2;
int msgPending = 0;
int autoclearwait = 1;
char *line, *line2, *text2;
int linecnt = 0;
float linespacing = 10.0;
int msgPauseDrawing = 0;
struct Message firstmsg, *curmsg, *lastmsg;

void destroyLastMsg (void) {
	curmsg = lastmsg;
	if (curmsg->prev == NULL) {	// don't free the root msg copy ('firstmsg') !
		msgPending = 0;		// Nothing will be called from drawing in render()
		memset (curmsg->text, 0, MESSAGESIZE-1);
		return;
	}
	if (curmsg->text != NULL) {
		free (curmsg->text);
		curmsg->text = NULL;
	}
	curmsg = curmsg->prev;
	curmsg->next->prev = NULL;
	if (curmsg->next != NULL) {
		free (curmsg->next);
		curmsg->next = NULL;
	}
	lastmsg = curmsg;
	--msgPending;
}

void decayMsg (void) {
	if (firstmsg.next == NULL) {	// Nothing to decay anymore
		msgPending = 0;		// Nothing will be called for drawing in render()
		memset (firstmsg.text, 0, MESSAGESIZE);
		return;
	}
	curmsg = &firstmsg;
	while (curmsg->next != NULL) {
		sprintf (curmsg->text, "%s", curmsg->next->text);
		curmsg = curmsg->next;
	}
	destroyLastMsg ();
}

void clearMsg (void) {
	curmsg = lastmsg;
	while (curmsg->prev != NULL) {
			memset (curmsg->text, 0, MESSAGESIZE);
			free (curmsg->text);
			curmsg = curmsg->prev;
			curmsg->next->prev = NULL;
			free (curmsg->next);
			curmsg->next = NULL;
	}
	lastmsg = curmsg;
	memset (curmsg->text, 0, MESSAGESIZE);
	msgPending = 0;
	needRedraw = 1;
}

void addMsg (int verbose, char *text) {
	curmsg = lastmsg;
	// just refill the 'firstmsg' copy if there is no msg
	if (curmsg->prev == NULL && msgPending == 0) {
			sprintf (curmsg->text, "%s", text);
			lastmsg = curmsg;
	}
	else {
		curmsg->next = malloc (sizeof (struct Message));
		curmsg->next->prev = curmsg;
		curmsg = curmsg->next;
		curmsg->text = malloc (MESSAGESIZE);
		memset (curmsg->text, 0, MESSAGESIZE);
		sprintf (curmsg->text, "%s", text);
		curmsg->rank = curmsg->prev->rank + 1;
		curmsg->next = NULL;
		lastmsg = curmsg;
	}
	++msgPending;
	autoclearwait = 4;
	needRedraw = 1;
}

