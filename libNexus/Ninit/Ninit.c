#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sensors/sensors.h>
#include <GL/glut.h>
#include <GL/glx.h>

#include "browser.h"
#include "buffer.h"
#include "camera.h"
#include "commands.h"
#include "config.h"
#include "osdata.h"
#include "displayList.h"
#include "drawing.h"
#include "events.h"
#include "getargs.h"
#include "hud.h"
#include "Ninit.h"
#include "Nsignals.h"
#include "NX.h"
#include "memfiller.h"
#include "memoryViewer.h"
#include "menu.h"
#include "messages.h"
#include "lexic.h"
#include "libNexus.h"
#include "objects.h"
#include "restore.h"
#include "selectID.h"
#include "state.h"
#include "tester.h"
#include "threading.h"
#include "util.h"

int winWidth = 800;
int winHeight = 600;
int program_nice = 0,
	program_pid,
	program_ppid,
	*program_argc;
char **program_argv, *program_name, *program_pwd;

void NLinit (int *argc, char **argv, char *winTitle) {
	NLshowVersion ();
	program_pid = getpid();
	program_ppid = getppid();
	printf ("Process ID: %d\n", program_pid);
	printf ("Parent ID: %d\n", program_ppid);
	program_argc = argc;
	program_argv = argv;
	program_name = program_argv[0];
	program_pwd = get_current_dir_name ();	// rem func is from unistd.h!
	//printf ("Session ID: %d\n", getsid (program_pid));
	//printf ("Parent session ID: %d\n", getsid (program_ppid));
	
	//NLinitState ();
	state.stateStr = malloc (stateStrSize);
	memset (state.stateStr, '!', stateStrSize);

	state.codeStr = malloc (stateStrSize);
	memset (state.codeStr, '@', stateStrSize);

	NLsetState (STATE_IDLE, STATE_OK);
	
	
	NLUinitSignals (NLsignalHandler);
	NLinitConfig ();
	NLgetargs ();
	//pthread_create (&drawingThread, NULL, drawingThreadFunc, NULL);
	//pthread_detach (drawingThread);
	//NLinitX ();
	NLinitGlut (winTitle);
	NLinitNexusEngine ();
	//printf ("NLinit() done @%ld ticks\n", clock());
	//printf ("NLinit() done in glutGet(): %d\n", glutGet  (GLUT_ELAPSED_TIME));
	glutMainLoop ();
}

void NLinitGlut (char *winTitle) {
	NLsetState (STATE_GLUTINIT, STATE_OK);
	glutInit (program_argc, program_argv);
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition (1024-winWidth,0);
	glutInitWindowSize (winWidth, winHeight);
	glutCreateWindow (winTitle);
	glutSetIconTitle ("Icon here");
	//glutSetCursor (GLUT_CURSOR_CROSSHAIR);
	//glutSetCursor (GLUT_CURSOR_NONE);
	cursorX = winWidth/2;
	cursorY = winHeight/2;
	//glutWarpPointer (cursorX, cursorY);
	
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor (0.0, 0.0, 0.0,  0.0);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glShadeModel (GL_FLAT);
	glEnable (GL_CULL_FACE);
	//glXWaitGL ();
	glEnable (GL_NORMALIZE);
	
	glutReshapeFunc (reshape);
	glutDisplayFunc (render);
	glutIdleFunc (idleFunc);
	glutKeyboardFunc (normalKeysDown);
	glutKeyboardUpFunc (normalKeysUp);
	glutSpecialFunc (specialKeysDown);
	glutSpecialUpFunc (specialKeysUp);
	glutMouseFunc (mouseFunc);
	glutMotionFunc (mouseMotionFunc);
	glutPassiveMotionFunc (mousePassiveMotionFunc);
//	glutSetKeyRepeat (GLUT_KEY_REPEAT_OFF);
	glutEntryFunc (entryFunc);
	//glutVisibilityFunc (visibilityFunc);
	glutWindowStatusFunc (windowFunc);
}

void NLinitNexusEngine (void) {
	NLsetState (STATE_ENGINEINIT, STATE_OK);
	initBuffering ();
	NLinitLexic ();
	initCamera ();
	initSelectID ();
	initDisplayListTree ();
	initHud ();
	initCommands ();
	initDrawing ();
	initObjects ();
	initMemoryViewer ();
	initOSData ();
	initMenu ();
	//initEditor ();
	initBrowser ();
	initThreading ();
	clearMsg ();
	turnAlpha (options.useAlpha);
}

void exitNexus (int stateNum, int codeNum, const char *caller) {
	NLsetState (stateNum, codeNum);
	printf ("exitNexus(): Called from %s()\n", caller);
	NLshowState ();
	if (sensorsChipName != NULL) {
		sensors_cleanup ();
	}
	exit (0);
}

void atExitNexus (void) {
	if (state.stateNum == STATE_ROOTCHK) {
		printf ("Need to be root\n");
		return;
	}

	if (state.codeNum == STATE_ERROR) {
		printf ("atExitNexus(): Exit error messages:\n");
		switch (state.stateNum) {
			case STATE_ARGS:
				errno = EINVAL;
				perror ("\tperror");
				break;
			case STATE_CONFIG:
				printf ("\tSomething wrong in the program's config\n");
				break;
			case STATE_FUNCTION:
				printf ("\tSomething wrong with a parent function\n");
				break;
			case STATE_SIGNAL:
				printf ("\tAn OS signal terminated the process\n");
				break;
			case STATE_TESTER:
				printf ("\tSomething wrong in the tester\n");
				break;
			default:
				printf ("\tExit on unknown state\n");
				break;
		}
	}
	else if (state.codeNum == STATE_WAITING) {
		printf ("atExitNexus(): A part of the program was still in a waiting state\n");
	}
}

