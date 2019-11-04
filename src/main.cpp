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

void Redraw(void) {
	gl->Clear();

	cgl::cgl_angle d = { 0 * 3.141592f / 180, 100 * 3.141592f / 180};
	cgl::cgl_point u1 = {10, 10};
	cgl::cgl_point v1 = {100, 100};
	cgl::cgl_color c1 = {0, 0, 255, 80};
	gl->AddLine(&u1, &v1, &c1);
	gl->AddArc(&u1, &v1, &d, &c1);

	cgl::cgl_point u2 = {20, 20};
	cgl::cgl_point v2 = {20, 80};
	cgl::cgl_point w2 = {80, 20};
	cgl::cgl_color c2 = {0, 255, 0, 80};
	gl->AddTriangle(&u2, &v2, &w2, &c2);


	cgl::cgl_point u3 = {10, 10};
	cgl::cgl_point v3 = {100, 10};
	cgl::cgl_point w3 = {10, 100};
	cgl::cgl_color c3 = {255, 0, 0, 80};
	gl->AddTriangle(&u3, &v3, &w3, &c3);

	cgl::cgl_point u4 = {20, 20};
	cgl::cgl_point v4 = {90, 90};
	cgl::cgl_color c4 = {0, 255, 255, 80};
	gl->AddEllipse(&u4, &v4, &c4);

	gl->Render();
}

int main(int argc, char **argv) {
	Display *display;
	Window window;
	XVisualInfo *vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	XEvent event;
	int dummy;
	KeySym keysym;
	char buffer;
	bool update = false;

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

			switch (event.type)
			{

			case KeyPress:
				/* It is necessary to convert the keycode to a keysym before checking if it is an escape */
				if ((XLookupString(&event.xkey, &buffer, 1, &keysym, NULL) == 1) && (keysym == XK_Escape))
					exit(0);
				break;

			case ButtonPress:
				/* Mouse button */
				switch (event.xbutton.button)
				{

				case 1:
					break;

				case 2:
					break;

				case 3:
					break;

				}
				break;

			case ConfigureNotify:
				/* Window changed */
				gl->SizeChanged();
				update = true;
				break;

			case Expose:
				/* Window need redraw */
				update = true;
				break;

			}
		} while (XPending(display)); /* loop to compress events */

		/* Redraw everything */
		if (update)
			Redraw();
	}

	return 0;
}
