#ifndef NEXUS_H
#define NEXUS_H 1

//#include <camera.h>
//#include <X11/Xlib.h>

extern const char *NEXUS_VERSION;
#include "libNexus.h"
/*
extern int dbgmsgcnt;
extern int winWidth, winHeight;
extern int program_nice;
extern int program_pid;
extern int program_ppid;
extern int program_argc;
extern char **program_argv;
extern char *program_name;
extern char *program_pwd;
//extern Display *display;
extern int exitProgram;
*/
/* This struct is used by saveState()
 All variables in the program will point to here.
 I tought it would be easier that way to add restorable vars */
 // ps: to be approved, I still doubt it could be a pain too,
 //		only camera.c use it
/*struct ProgramVariables {
	Camera camera;
} pv;
*/
void nexusUsage (void);

#endif /* NEXUS_H */
