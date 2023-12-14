#include <stdio.h>
#include <unistd.h>

#include <X11/Xlib.h>

#include "Ninit.h"
#include "state.h"

Display *display;
Screen *screen;
Visual *visual;
XSetWindowAttributes *winattr;
Window window;

void NLinitX (void) {
	display = XOpenDisplay (XDisplayName (NULL));
	if (display == NULL) {
		printf ("NLinitX(): Cannot connect to X\n");
		perror ("perror()");
		exitNexus (STATE_XINIT, STATE_ERROR, __FUNCTION__);
	}
	
	screen = DefaultScreenOfDisplay (display);
	if (screen == NULL) {
		printf ("NLinitX(): Cannot get display's default screen\n");
		exitNexus (STATE_XINIT, STATE_ERROR, __FUNCTION__);
	}
	printf ("\tscreenNumber: %d\n", XScreenNumberOfScreen (screen));
	printf ("\tdimensions: %dx%d\n", WidthOfScreen (screen), HeightOfScreen (screen));
	printf ("\tdepth: %d\n", DefaultDepthOfScreen (screen));
	
	visual = DefaultVisualOfScreen (screen);
	if (visual == NULL) {
		printf ("NLinitX(): Cannot get the default visual\n");
	}
	printf ("rootWindow2: %d\n", (int)DefaultRootWindow (display));
	window = XCreateSimpleWindow (display, DefaultRootWindow (display), 20,20,
							800,600, 1, 0, 0);
	printf ("new window: %d\n", (int)window);
	//exitNexus (STATE_XINIT, STATE_OK, __FUNCTION__);
}

