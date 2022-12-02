// Source File by: Axel Arcos
// Date: 09/13/2022
// CS 3350

#include <unistd.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include "GameObjects/GameShared.h"
namespace aarcosavalos{

    Rect r, t;
    unsigned int manage_state(unsigned int s)
    {
        s = s ^ 1;
        return s;
    }

    void finish_game (int xres, int yres)
    {
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

        r.bot = yres-40;
        r.left = xres/2.5;
        r.center = 0;

        ggprint16(&r, 16, 0x00ff0000, "GAME OVER");
        ggprint16(&r, 16, 0x00ff0000, "Press Esc to stop the game");
        ggprint16(&r, 16, 0x00ff0000, "Play Again? (Y/ESC)");

    }
    void Feature_mode(int xres, int yres)
    {
        int w = xres;
        r.bot = yres -30;
        r.left = 10;
        r.center = 0;

        t.bot = yres-40;
        t.left = xres/2.5;
        t.center = 0;

        // Draw a border using a triangle strip
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glColor4f(1.0, 1.0, 1.0, 0.25);
        
        w = 20;

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
        ggprint16(&t, 16, 0x00ffffff, "WELCOME TO MY FEATURE");
        ggprint8b(&r, 16, 0x00ffffff, "Press g to see the first feature");
        ggprint8b(&r, 16, 0x00ffffff, "Press t to see the first feature");

    }
    void intro_screen(int xres, int yres)
    {
        t.bot = yres/2;
        t.left = xres/2.5;
        t.center = 0;

        glClear(GL_COLOR_BUFFER_BIT);
        // Draw box
        glPushMatrix();
        glColor3ub(0, 0, 0);
        glBegin(GL_QUADS);
        glVertex2f(-xres, -yres);
        glVertex2f(-xres,  yres);
        glVertex2f( xres,  yres);
        glVertex2f( xres, -yres);

        glBindTexture(GL_TEXTURE_2D, 0);
        glEnd();
        glPopMatrix();

        ggprint16(&t, 16, 0x00ffffff, "WELCOME TO TOP GUN");
        ggprint16(&t, 16, 0x00ffffff, "Press I to start the game");
        ggprint16(&t, 16, 0x00ffffff, "WASD to move, SPACE to shoot");
        ggprint16(&t, 16, 0x00ffffff, "1 - 5 to change weapons");
        ggprint16(&t, 16, 0x00ffffff, "P to pause the game");
        ggprint16(&t, 16, 0x00ffffff, "F1 to enter the help screen");

    }
     void change_shape(int num)
    {
        /// Pistol
        if (num == 1)
        {
            glBegin(GL_TRIANGLES);
            glVertex2f(-12.0f, -10.0f);
            glVertex2f(  0.0f,  20.0f);
            glVertex2f(  0.0f,  -6.0f);
            glVertex2f(  0.0f,  -6.0f);
            glVertex2f(  0.0f,  20.0f);
            glVertex2f( 12.0f, -10.0f);
            glEnd();
        }
        // Rifle
        if (num == 2)
        {
            glBegin(GL_TRIANGLES);
            glVertex2f(-12.0f, -20.0f);
            glVertex2f(  0.0f,  20.0f);
            glVertex2f(  0.0f,  -6.0f);
            glVertex2f(  0.0f,  -6.0f);
            glVertex2f(  0.0f,  20.0f);
            glVertex2f( 12.0f, -20.0f);
            glEnd();
        }
        // Shotgun
        if (num == 3)
        {
            glBegin(GL_QUADS);
            glVertex2f( 12.0f,  10.0f);
            glVertex2f( 12.0f, -10.0f);
            glVertex2f(-12.0f,  10.0f);
            glVertex2f(-12.0f, -10.0f);
            glEnd();
        }
        // Sniper
        if (num == 4)
        {
            glBegin(GL_TRIANGLES);
            glVertex2f(-12.0f, -40.0f);
            glVertex2f(  0.0f,  40.0f);
            glVertex2f(  0.0f, -10.0f);
            glVertex2f(  0.0f, -10.0f);
            glVertex2f(  0.0f,  40.0f);
            glVertex2f( 12.0f, -20.0f);
            glEnd();
        }
        // Machine Gun
        if (num == 5)
        {
            glBegin(GL_QUADS);
            glVertex2f( 12.0f,  40.0f);
            glVertex2f( 12.0f, -40.0f);
            glVertex2f(-12.0f,  40.0f);
            glVertex2f(-12.0f, -40.0f);
            glEnd();
        }
    }
}
