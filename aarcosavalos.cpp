// Source File by: Axel Arcos
// Date: 09/13/2022
// CS 3350

#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"

namespace aarcosavalos{

    unsigned int manage_state(unsigned int s)
    {
        s = s ^ 1;
        return s;
    }

    void finish_game (int xres, int yres)
    {
        Rect r;
        int xcent = xres;
        int ycent = yres;
        int w = xres;
        glClear(GL_COLOR_BUFFER_BIT);
        // This prints a colo red
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_QUADS);
            glVertex2f(xcent-w, ycent-w);
            glVertex2f(xcent-w, ycent+w);
            glVertex2f(xcent+w, ycent+w);
            glVertex2f(xcent+w, ycent-w);
        glEnd();

        r.bot = yres -20;
        r.left = 10;
        r.center = 0;
        ggprint8b(&r, 16, 0x00ff0000, "GAME OVER");
        ggprint8b(&r, 16, 0x00ff0000, "Press Esc to stop the game");
        ggprint8b(&r, 16, 0x00ff0000, "Play Again? (Y/ESC)");
    }
}
