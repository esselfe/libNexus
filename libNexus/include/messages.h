#ifndef MESSAGES_H
#define MESSAGES_H 1

struct Message {
	char *text;
	short rank;
	struct Message *prev, *next;
} firstmsg, *curmsg, *lastmsg;
extern const short MESSAGESIZE;
extern char *msgbuf;	// Mostly for addMsg(), for special sprintf() formatting needs, used as needed
extern char *msgbuf2;	// Idem, but used by events and threads... may change anytime
extern int msgPending;	// How many messages are there.
extern int autoclearwait;	// Just set it to nseconds and decayMsg() will decrease it instead of 
							// decaying the msg stack 
extern char *line, *line2, *text2;
extern int linecnt;
extern float linespacing;
extern int msgPauseDrawing;

void destroyLastMsg (void);
void decayMsg (void);
void clearMsg (void);
void addMsg (int verbose, char *text);

#endif /* MESSAGES_H */

