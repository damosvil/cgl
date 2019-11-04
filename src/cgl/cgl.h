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
	void AddLine(float x1, float y1, float x2, float y2, float R, float G, float B, float A);
	void AddTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float R, float G, float B, float A);
	void AddRectangle(float x1, float y1, float x2, float y2, float R, float G, float B, float A);
	void AddBox(float x1, float y1, float x2, float y2, float R, float G, float B, float A);
	void AddEllipse(float x1, float y1, float x2, float y2, float R, float G, float B, float A);

};

} /* namespace cgl */

#endif /* CGL_CGL_H_ */
