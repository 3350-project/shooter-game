/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for drawing on screen.
*/
#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
#include "rwyatt.h"

/*
 * STATIC METHODS
 */
RWyatt::RWyatt() {}

void RWyatt::flipState(bool &state)
{
    state = !state;
}

void RWyatt::pauseScreen(bool &paused) 
{
    RWyatt::flipState(paused);
}

void RWyatt::drawBorder(int xres, int yres)
{
    // draw a border using a triangle strip
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 0.0, 0.3);
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

void RWyatt::drawScore(int xres, int yres, int score)
{
	Rect r;
	r.bot = yres - 100;
	r.left = xres / 2.5;
	r.center = 0;

    ggprint16(&r, 20, 0x00ffffff, "Score: %d", score);
}
