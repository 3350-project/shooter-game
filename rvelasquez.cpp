//name: Rodolfo Velasquez 
//file: rvelasquez.cpp
//
//Team 3 credits screen
#include <X11/Xlib.h>
#include <GL/glx.h>

unsigned int managed_state(unsigned int c)
{
	c = c ^ 1;
	return c;
}

void show_credits(int xres, int yres)
{	
	int xcent = xres / 2;
	int ycent = yres / 2;
	int w = 200;
	glColor3f(0.57, 0.82, 1.0);
	glBegin(GL_QUADS);
		glVertex2f(xcent-w, ycent-w);
		glVertex2f(xcent-w, ycent+w);
		glVertex2f(xcent+w, ycent+w);
		glVertex2f(xcent+w, ycent-w);
	glEnd();
}


