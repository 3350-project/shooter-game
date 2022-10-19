//Source file by: Steven Nez
//Date: October 3, 2022
//
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>

namespace snez
{

Rect r;
float PI;
unsigned int manage_stateF2(unsigned int F2)
{
    F2 = F2 ^ 1;
    return F2;
}

unsigned int manage_stateF1(unsigned int F1)
{
    F1 = F1 ^ 1;
    return F1;
}
void Show_HelpScr(int xres, int yres)
{
    int xcent = xres;
    int ycent = yres;
    int w = xres;
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glVertex2f(xcent-w, ycent-w);
        glVertex2f(xcent-w, ycent+w);
        glVertex2f(xcent+w, ycent+w);
        glVertex2f(xcent+w, ycent-w);
        glEnd();
    r.bot = yres - 20;
    r.left = 10;
    r.center = 0;
    ggprint8b(&r, 16, 0xffffffff, "HELP SCREEN");
    ggprint8b(&r, 16, 0xffffffff, "Controls: W: Turn on Thrusters and go forwards, D: Turn Right, A: Turn Left");
}
//This will be my Function that will detect collision and will coincide with taking damage aswell
void collision_detection(int xres, int yres) 
{
    if (xres == 100) { 
    } 
    if (yres == 100) { 
    }

    float PI = 3.14159; 
    glBegin(GL_POINTS); 
    for (int i = 0; i < 1000; i++) { 
	glVertex3f(cos(2 * PI * i/1000.0), sin(2 * PI * i/1000.0),0); 
    } 
    glEnd(); 
    r.bot = yres - 20; 
    r.left = 10; 
    r.center = 0; 
    ggprint8b(&r, 16, 0xffffffff, "Collision Detection Test");
}
//This will be my Ai function that allows the enemies to follow the location of player
//void Ai(){
//
//}
}
