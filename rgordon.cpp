// Ryan Gordon
// Individual Source File
// 9/14/22
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
}