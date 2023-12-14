#ifndef UTIL_H
#define UTIL_H 1

// Command ran  by the parent process on user's demand
extern char *parentcmd;
//extern char *cwd;
extern const int CWDSIZE;

// Shows text when the 'once64' value is set. the o key toggle the flag. Useful for drawing loops and such...
void msg64 (char *text);

#define VLEVEL_QUIET	0
#define VLEVEL_NORMAL	1
#define VLEVEL_INFO		2
#define VLEVEL_DEBUG	3
void vmsg (int verboseLevel, char *text);
// used as 'dgbFuncMsg (1, __FUNC__);' for start of func or set 0
void dbgFuncMsg (int setval, char *funcname);
void dbgmsg (char *str);
void beep (int vol, int pitch, int duration);
void *terminalThreadFunc (void *arg);
void parentProcessRuncmd (char *str);	// Useful to run cmd in the background

/* Will set the escape sequence to the terminal */
#define NNORMAL		0
#define NRED		1
#define NGREEN		2
#define NBLUE		3
#define NBRED		11
#define NBGREEN		12
#define NBBLUE		13
void setTerminalColor (int newColor);


void NLUinitSignals (void (*signalHandler)(int signum));

#endif /* UTIL_H */
