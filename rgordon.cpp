// Ryan Gordon
// Individual Source File
// Team 3
// intro
//
//
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include "rgordon.h"

namespace rgordon{
	
unsigned int manage_state(unsigned int i)
{
	i = i ^ 1;
	return i;
}

unsigned int manage_state_weapon(unsigned int F3)
{
	F3 = F3 ^ 1;
	return F3;
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

void weapon(int xres, int yres)
{
	Rect s; 
	s.bot = yres - 100;
    s.left = xres/3;
    s.center = 0;

	ggprint16(&s, 16, 0x00ffffff, "Ryan's Weapon Test Feature");

    // draw a border using a triangle strip
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 1.0, 0.3);
    int w = 20;
    glBegin(GL_TRIANGLE_STRIP);
        glVertex2i(0, 0);
        glVertex2i(0+w, w);
        glVertex2i(0, yres);
        glVertex2i(0+w, yres-w);
        glVertex2i(xres, yres);
        glVertex2i(xres-w, yres-w);
        glVertex2i(xres, 0);
        glVertex2i(xres-w, w);
        glVertex2i(0, 0);
        glVertex2i(0+w, w);
    glEnd();
    glDisable(GL_BLEND);
}



}
