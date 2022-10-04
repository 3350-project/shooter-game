//Source file by: Steven Nez
//Date: October 3, 2022
//

#include <iostream>
    
unsigned int manage_stateF1(unsigned int F1)
{
    F1 = F1 ^ 1;
    return F1;
}
void ShowHelpScr()
{
    int xcent = gl.xres;
    int ycent = gl.yres;
    int w = 200;
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
        glVertiex2f(xcent-w, ycent-w);
        glVertiex2f(xcent-w, ycent+w);
        glVertiex2f(xcent+w, ycent+w);
        glVertiex2f(xcent+w, ycent-w);
        glEnd();
    r.bot = gl.yres - 20;
    r.left = gl.xres / 2;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ff0000, "HELP SCREEN");
    ggprint8b(&r, 16, 0x00ff0000, "Controls: W: Up, D: Right, A: Left, S: Down ");
}

