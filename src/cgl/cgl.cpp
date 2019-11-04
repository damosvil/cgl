/*
 * cgl.cpp
 *
 *  Created on: 4 nov. 2019
 *      Author: iso9660
 */

#include <float.h>
#include <math.h>
#include "cgl.h"


using namespace std;


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
//	float vAmbientLightBright[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	XWindowAttributes wa;
	GLXContext cx;

	if (d == NULL || i == NULL || w == 0)
		throw "Some of the given inputs is NULL";
#include <float.h>

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

	/* No depth is necessary for 2D */
//	/* Prepare depth buffer */
//	glEnable(GL_DEPTH_TEST); /* enable depth buffering */
//	glDepthFunc(GL_LESS); /* pedantic, GL_LESS is the default */
//	glClearDepth(1.0); /* pedantic, 1.0 is the default */

	/* Select glClear color */
	glClearColor(1.0, 1.0, 1.0, 1.0);

	/* Activate OpenGL color blending */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* No lighting is necessary for 2D */
//	glEnable(GL_LIGHTING);
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vAmbientLightBright);
//	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE) ;
//	glEnable(GL_COLOR_MATERIAL);

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
	/* Depth buffer not necessary for 2D */
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	/* No model view reinitialization necessary for 2D */
//	/* Initialize model view */
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
}

void cgl::Render()
{
	// Double buffer swap
	glXSwapBuffers(m_display, m_window);
}

void cgl::AddLine(const cgl_point *p1, const cgl_point *p2, const cgl_color *c)
{
	glColor4ub(c->r, c->g, c->b, c->a);

	glBegin(GL_LINES);
	glVertex2f(p1->x, p1->y);
	glVertex2f(p2->x, p2->y);
	glEnd();
}

void cgl::AddTriangle(const cgl_point *p1, const cgl_point *p2, const cgl_point *p3, const cgl_color *c)
{
	glColor4ub(c->r, c->g, c->b, c->a);

	glBegin(GL_TRIANGLES);
	glVertex2f(p1->x, p1->y);
	glVertex2f(p2->x, p2->y);
	glVertex2f(p3->x, p3->y);
	glEnd();
}

void cgl::AddRectangle(const cgl_point *p1, const cgl_point *p2, const cgl_color *c)
{
	glColor4ub(c->r, c->g, c->b, c->a);

	glBegin(GL_LINES);
	glVertex2f(p1->x, p1->y);
	glVertex2f(p2->x, p1->y);
	glVertex2f(p2->x, p2->y);
	glVertex2f(p1->x, p2->y);
	glVertex2f(p1->x, p1->y);
	glEnd();
}

void cgl::AddBox(const cgl_point *p1, const cgl_point *p2, const cgl_color *c)
{
	glColor4ub(c->r, c->g, c->b, c->a);

	glBegin(GL_TRIANGLES);
	glVertex2f(p1->x, p1->y);
	glVertex2f(p2->x, p1->y);
	glVertex2f(p2->x, p2->y);
	glVertex2f(p1->x, p1->y);
	glVertex2f(p2->x, p2->y);
	glVertex2f(p1->x, p2->y);
	glEnd();
}

void cgl::AddArc(const cgl_point *p1, const cgl_point *p2, const cgl_angle *a, const cgl_color *c)
{
	float cx = (p1->x + p2->x) / 2, cy = (p1->y + p2->y) / 2;
	float rx = (p2->x - p1->x) / 2;
	float ry = (p1->y - p2->y) / 2;
	float adelta = ((a->end < a->start) ? a->end + 360 - a->start : a->end - a->start) / 64;
	float aa = a->start;
	cgl_point u, v;

	u.x = cx + rx * sin(aa);
	u.y = cy + ry * cos(aa);
	aa += adelta;

	for (uint32_t i = 1; i <= 64; i++)
	{
		v.x = cx + rx * sin(aa);
		v.y = cy + ry * cos(aa);
		aa += adelta;

		AddLine(&u, &v, c);
		u = v;
	}
}

void cgl::AddEllipse(const cgl_point *p1, const cgl_point *p2, const cgl_color *c)
{
	cgl_angle d = { 0, 2 * 3.141592f};
	AddArc(p1, p2, &d, c);
}



} /* namespace cgl */
