// Reid Wyatt
#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
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

/*
 * CLASS METHODS
 */
RWyatt::RWyatt() {
    network_state = false;
}

bool RWyatt::start_networking() 
{
    // check multiplayer bool
    this->network_state = true;
    // TODO: network client
    return EXIT_FAILURE;
}

bool RWyatt::stop_networking() 
{
    // check multiplayer bool
    this->network_state = false;
    // TODO: network client
    return EXIT_FAILURE;
}

bool RWyatt::networked()
{
    return network_state;
}

bool RWyatt::mutliplayer()
{
    return multiplayer;
}

std::string RWyatt::player_name()
{
    return user_name;
}

std::string RWyatt::server_name()
{
    return server_ip;
}

void RWyatt::draw_networking(int xres, int yres)
{
	Rect r;
	r.bot = yres - 80;
	r.left = xres / 2;
	r.center = 0;

    ggprint16(&r, 20, 0x00ffffff, "Reid's Multiplayer Feature");

    if (this->multiplayer) {
        ggprint16(&r, 20, 0x00ffffff, "Multiplayer Enabled");
        ggprint16(&r, 20, 0x0000ff00, "Server: %s", 
                this->server_name().c_str());
        ggprint16(&r, 20, 0x0000ff00, "Name: %s", 
                this->player_name().c_str());
        ggprint16(&r, 20, 0x0000ff00, "Status: %s", 
                this->networked() ? "Connected" : "Disconnected");
    } else {
        ggprint16(&r, 20, 0x00ff0000, "Multiplayer disabled");
    }
}

void RWyatt::set_network_config(int argc, char* argv[])
{
    if (argc < 2) {
        this->multiplayer = false;
        return;
    }

    this->multiplayer = true;
    this->server_ip = argv[1];
    this->user_name = argv[2];
}
