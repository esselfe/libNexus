#ifndef NINIT_H
#define NINIT_H 1

#include "camera.h"

extern int dbgmsgcnt;
extern int winWidth, winHeight;
extern int program_ticks;	// Set in timerThreadFunc()
extern int program_nice;
extern int program_pid;
extern int program_ppid;
extern int *program_argc;
extern char **program_argv;
extern char *program_name;
extern char *program_pwd;
//extern Display *display;
extern int exitProgram;

/* This struct is used by saveState()
 All variables in the program will point to here.
 I tought it would be easier that way to add restorable vars */
 // ps: to be approved, I still doubt it could be a pain too,
 //		only camera.c use it
struct ProgramVariables {
	Camera camera;
};
extern struct ProgramVariables pv;

void NLinit (int *argc, char **argv, char *winTitle);


// Private
void NLinitNexusEngine (void);
void NLinitGlut (char *winTitle);

/* Used to quit the program */
// example: exitNexus (STATE_TESTER, STATE_OK, __FUNCTION__);
void exitNexus (int stateNum, int codeNum, const char *caller);

/* Registered by atexit() from unistd.h */
void atExitNexus (void);

#endif /* NINIT_H */
