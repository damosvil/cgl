/*
 * cgl.h
 *
 *  Created on: 4 nov. 2019
 *      Author: iso9660
 */

#ifndef CGL_CGL_H_
#define CGL_CGL_H_


#include <GL/glx.h>
#include <GL/gl.h>


namespace cgl {

struct cgl_color
{
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct cgl_point
{
	float x;
	float y;
};

struct cgl_angle
{
	float start;
	float end;
};

class cgl
{

private:
	Display *m_display;
	Window m_window;
	GLXContext m_context;
	XVisualInfo *m_xvisual;

public:
	cgl();
	virtual ~cgl();

	void Attach(Display *d, Window w, XVisualInfo *i);
	void SizeChanged();
	void Clear();
	void Render();
	void AddLine(const cgl_point *p1, const cgl_point *p2, const cgl_color *c);
	void AddTriangle(const cgl_point *p1, const cgl_point *p2, const cgl_point *p3, const cgl_color *c);
	void AddRectangle(const cgl_point *p1, const cgl_point *p2, const cgl_color *c);
	void AddBox(const cgl_point *p1, const cgl_point *p2, const cgl_color *c);
	void AddEllipse(const cgl_point *p1, const cgl_point *p2, const cgl_color *c);
	void AddArc(const cgl_point *p1, const cgl_point *p2, const cgl_angle *a, const cgl_color *c);

};

} /* namespace cgl */

#endif /* CGL_CGL_H_ */
