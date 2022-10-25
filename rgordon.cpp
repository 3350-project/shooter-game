// Ryan Gordon
// Individual Source File
// Team 3
// intro
//
//
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
namespace rgordon{
	
unsigned int manage_state(unsigned int i)
{
	i = i ^ 1;
	return i;
}
	
void intro(int xres, int yres)
{	
	Rect r;
    glClear(GL_COLOR_BUFFER_BIT);
	int xcent = xres / 2;
	int ycent = yres / 2;
	int w = 200;
	glColor3f(0.3, 0.1, 0.8);
	glBegin(GL_QUADS);
		glVertex2f(xcent-w, ycent-w);
		glVertex2f(xcent-w, ycent+w);
		glVertex2f(xcent+w, ycent+w);
		glVertex2f(xcent+w, ycent-w);
	glEnd();
	r.bot = ycent;
    r.left = xcent;
    r.center = ycent + xcent;
	//print welcome
	ggprint8b(&r, 16, 0x00ff0000, "Intro Screen");
}

/*  in progress


unsigned int manage_state_test_mode(unsigned int p)
{
	p = p ^ 1;
	return p;
}
void test_mode(int xres, int yres)
{
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glColor3f(1.0, 1.0, 0.0);
		glColor4f(1.0, 1.0, 0.0, 0.9);
		int w = 20;
		glBegin(GL_TRIANGLE_STRIP);
			glVertex2i(0, 0);
			glVertex2i(w, w);
			
			glVertex2i(0, gl.yres);
			glVertex2i(w, gl.yres-w);

			glVertex2i(gl.xres, gl.yres);
			glVertex2i(gl.xres-w, gl.yres - w);
			
			glVertex2i(gl.xres, 0);
			glVertex2i(gl.xres-w, w);
			
			glVertex2i(0, 0);
			glVertex2i(w, w);
	glEnd();
	glDisable(GL_BLEND);
}

*/

}
