//============================================================================
// Name        : cgl.cpp
// Author      : ISO9660
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */
#include <stdio.h>
#include <stdlib.h>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include "cgl/cgl.h"


/* No depth buffer necessary for 2D */
static int dblBuf[] = { GLX_RGBA, /* GLX_DEPTH_SIZE, 16, */ GLX_DOUBLEBUFFER, None };

/* cgl object */
static cgl::cgl *gl = NULL;


void fatalError(const char *message) {
	fprintf(stderr, "main: %s\n", message);
	exit(1);
}

void redraw(void) {
	gl->Clear();
	gl->AddLine(10, 10, 100, 100, 0, 0, 1, 0.3);
	gl->AddTriangle(20, 20, 80, 20, 20, 80, 1, 0, 0, 1);
	gl->AddTriangle(10, 10, 100, 10, 10, 100, 0, 1, 0, 0.5);
	gl->Render();
}

int main(int argc, char **argv) {
	Display *display;
	Window window;
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	XEvent event;
	GLboolean needRedraw = GL_FALSE;
	int dummy;

	/*** (1) open a connection to the X server ***/

	display = XOpenDisplay(NULL);
	if (display == NULL)
		fatalError("could not open display");

	/*** (2) make sure OpenGL's GLX extension supported ***/

	if (!glXQueryExtension(display, &dummy, &dummy))
		fatalError("X server has no OpenGL GLX extension");

	/*** (3) find an appropriate visual ***/

	/* find an OpenGL-capable RGB visual with depth buffer */
	vi = glXChooseVisual(display, DefaultScreen(display), dblBuf);
	if (vi == NULL || vi->c_class != TrueColor) {
		fatalError("Double buffered trueColor visual required for this program");
	}

	/*** (4) create an X window with the selected visual ***/

	/* create an X colormap since probably not using default visual */
	cmap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
	swa.colormap = cmap;
	swa.border_pixel = 0;
	swa.event_mask = KeyPressMask | ExposureMask | ButtonPressMask | StructureNotifyMask;
	window = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 300,
			300, 0, vi->depth, InputOutput, vi->visual,
			CWBorderPixel | CWColormap | CWEventMask, &swa);
	XSetStandardProperties(display, window, "main", "main", None, argv, argc, NULL);

	/*** (5) request the X window to be displayed on the screen ***/

	XMapWindow(display, window);

	// Attach cgl
	gl = new cgl::cgl();
	gl->Attach(display, window, vi);

	while (1) {
		do {
			XNextEvent(display, &event);
			switch (event.type) {

			case KeyPress: {
				KeySym keysym;
				char buffer[1];

				/* It is necessary to convert the keycode to a keysym before checking if it is an escape */
				if ((XLookupString((XKeyEvent *) &event, buffer, 1, &keysym, NULL) == 1) && (keysym == (KeySym) XK_Escape))
					exit(0);
				break;
			}

			case ButtonPress:
				switch (event.xbutton.button) {
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				}
				break;

			case ConfigureNotify:
				gl->SizeChanged();
			case Expose:
				needRedraw = GL_TRUE;
				break;

			}
		} while (XPending(display)); /* loop to compress events */

		if (needRedraw) {
			redraw();
			needRedraw = GL_FALSE;
		}
	}

	return 0;
}
