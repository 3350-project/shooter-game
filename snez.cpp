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
    r.bot = yres - 100;
    r.left = xres/3;
    r.center = 0;
    ggprint8b(&r, 16, 0xffffffff, "HELP SCREEN");
    ggprint8b(&r, 16, 0xffffffff, "Controls: W: Turn on Thrusters and go forwards, D: Turn Right, A: Turn Left");
    ggprint8b(&r, 16, 0xffffffff, "To Go into Steven's Feature Mode Press F2");
}
//This will be my Function that will detect collision and will coincide with taking damage aswell
void collision_detection(int xres, int yres) 
{
    
    //int hp = 5;
    if (xres == 100) { 
    } 
    if (yres == 100) { 
    }
    float theta;
    glBegin(GL_POLYGON);
        for(int i = 0; i < 360; i++) {
            theta = i*3.142/180;
            glVertex2f(40*cos(theta), 40*sin(theta));
        }
	//if ship.pos[0] == asteroid.pos[0] && ship.pos[1] == asteroid.pos[1]) {
		//hp--;
	//}
	//if(hp == 0) {
	//aarcosavalos::finish.game(gl.xres, gl.yres);
	//} 
    glEnd();
}
//This will be my Ai function that allows the enemies to follow the location of player
//void Ai(){
//
//}
}
