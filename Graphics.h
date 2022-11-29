/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * File containing X11 Wrapper methods from asteroids.cpp.
 * Moved to this file for simplicity.
*/
#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <X11/Xlib.h>
#include <GL/glx.h>
#include "GameObjects/GameShared.h"

//X Windows variables
class X11_wrapper {
    private:
        Display *dpy;
        Window win;
        GLXContext glc;
        Global &gl;
    public:
        X11_wrapper(int w, int h, Global&);
        ~X11_wrapper();
        void set_title();
        void check_resize(XEvent *e);
        void reshape_window(int width, int height);
        void setup_screen_res(const int w, const int h);
        void swapBuffers();
        bool getXPending();
        XEvent getXNextEvent();
        void set_mouse_position(int x, int y);
        void show_mouse_cursor(const int onoff);
};

#endif
