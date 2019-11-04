/*
 * cgl.cpp
 *
 *  Created on: 4 nov. 2019
 *      Author: iso9660
 */

#include "cgl.h"

namespace cgl
{

cgl::cgl()
{
	m_display = NULL;
	m_window = 0;
	m_context = 0;
	m_xvisual = NULL;
}

cgl::~cgl()
{

}

void cgl::Attach(Display *d, Window w, XVisualInfo *i)
{
	XWindowAttributes wa;
	GLXContext cx;

	if (d == NULL || i == NULL || w == 0)
		throw "Some of the given inputs is NULL";

	XGetWindowAttributes(d, w, &wa);

	// Check a Truecolor visual is selected
	if (i->c_class != TrueColor)
		throw "cgl can only work in Truecolor windows";


	/* Create OpenGL context */
	cx = glXCreateContext(d, i, None, GL_TRUE);
	if (cx == NULL)
		throw "could not create rendering context";

	/* Bind context to window */
	glXMakeCurrent(d, w, cx);

	/* Prepare depth buffer */
	glEnable(GL_DEPTH_TEST); /* enable depth buffering */
	glDepthFunc(GL_LESS); /* pedantic, GL_LESS is the default */
	glClearDepth(1.0); /* pedantic, 1.0 is the default */

	/* Select glClear color */
	glClearColor(1.0, 1.0, 1.0, 1.0);

	/* Activate OpgnGL color blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	/* Configure projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, wa.width, wa.height, 0, 10, -1);

	/* Configure viewport */
	glViewport(0, 0, wa.width, wa.height);

	/* Initialize model view */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_display = d;
	m_window = w;
	m_context = cx;
	m_xvisual = i;
}

void cgl::SizeChanged()
{
	XWindowAttributes wa;

	XGetWindowAttributes(m_display, m_window, &wa);

	/* Configure projection */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, wa.width, wa.height, 0, 10, -1);

	/* Configure viewport */
	glViewport(0, 0, wa.width, wa.height);
}

void cgl::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void cgl::Render()
{
	// Double buffer swap
	glXSwapBuffers(m_display, m_window);
}

void cgl::AddLine(float x1, float y1, float x2, float y2, float R, float G, float B, float A)
{
	glColor4f(R, G, B, A);

	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glEnd();
}

void cgl::AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float R, float G, float B, float A)
{
	glColor4f(R, G, B, A);

	glBegin(GL_TRIANGLES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glVertex3f(x3, y3, 0);
	glEnd();
}

void cgl::AddRectangle(float x1, float y1, float x2, float y2, float R, float G, float B, float A)
{
	glColor4f(R, G, B, A);

	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y1, 0);
	glVertex3f(x2, y2, 0);
	glVertex3f(x1, y2, 0);
	glVertex3f(x1, y1, 0);
	glEnd();
}

void cgl::AddBox(float x1, float y1, float x2, float y2, float R, float G, float B, float A)
{
	glColor4f(R, G, B, A);

	glBegin(GL_TRIANGLES);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y1, 0);
	glVertex3f(x2, y2, 0);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glVertex3f(x1, y2, 0);
	glEnd();
}

void cgl::AddEllipse(float x1, float y1, float x2, float y2, float R, float G, float B, float A)
{

}


} /* namespace cgl */
