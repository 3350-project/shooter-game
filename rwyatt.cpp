// Reid Wyatt
#include <iostream>
#include <GL/glx.h>
#include "rwyatt.h"

/*
 * STATIC METHODS
 */
void RWyatt::flip_state(bool &state)
{
    state = !state;
}

void RWyatt::pause_screen(bool &paused) 
{
    RWyatt::flip_state(paused);
}

void RWyatt::draw_border(int xres, int yres)
{
    // draw a border using a triangle strip
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 0.0, 0.9);
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

/*
 * CLASS METHODS
 */
RWyatt::RWyatt() {
    network_state = false;
}

bool RWyatt::startNetworking() 
{
    this->network_state = true;

    // TODO: network client
    return EXIT_FAILURE;
}

bool RWyatt::networked()
{
    return network_state;
}
