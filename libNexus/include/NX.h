#ifndef NX_H
#define NX_H 1

#include <X11/Xlib.h>

extern Display *display;
extern Screen *screen;
extern Visual *visual;
extern XSetWindowAttributes *winattr;
extern Window window;

void NLinitX (void);

#endif /* NX_H */
