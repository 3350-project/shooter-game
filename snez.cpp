//Source file by: Steven Nez
//Date: October 3, 2022
//
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
namespace snez
{

    Rect r,s,d,f, i,it;
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
        r.left = xres/6;
        r.center = 0;

        ggprint8b(&r, 16, 0xffffffff, "HELP SCREEN");
        ggprint8b(&r, 16, 0xffffffff, "Controls:  W: To Go Up,  S: To Go Down, D: To Rotate Right, A: To Rotate Left, SPACE: To Shoot Bullets");
        ggprint8b(&r, 16, 0xffffffff, "To Go into Steven's Feature Mode Press F2");
        ggprint8b(&r, 16, 0xffffffff, "To Go into Rudy's Feature Mode Press S");

    }
    void Featuremode(int xres, int yres, int Collision, int hp, int nasteroids)
    {

        int xcent = xres;
        int ycent = yres;
        int w = xres;

        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.0, 0.0, 0.5);
        glBegin(GL_QUADS);
        glVertex2f(xcent-w, ycent-w);
        glVertex2f(xcent-w, ycent+w);
        glVertex2f(xcent+w, ycent+w);
        glVertex2f(xcent+w, ycent-w);
        glEnd();

        s.bot = yres-40;
        s.left = xres/2.5;
        s.center = 0;

        f.bot = yres-70;
        f.left = xres/2.5;
        f.center = 0;

        i.bot = yres/100;
        i.left = xres - (xres-50);
        i.center = 0;

        it.bot = (yres/100)+ 20;
        it.left = xres - (xres-50);
        it.center = 0;

        r.bot = yres - 50;
        r.left = 50;
        r.center = 0;

        d.bot = yres - 20;
        d.left = 10;
        d.center = 0;

        if(Collision == 1) {
            ggprint16(&f, 16, 0x00ff0000, "3350 - TopGun");
            ggprint16(&it, 16, 0x00ff0000, "Health Points Left: %i", hp);
            ggprint8b(&r, 16, 0x00ffff00, "Number of Enemies Left: %i", nasteroids);
            ggprint16(&s, 16, 0x00ffffff, "WELCOME TO MY FEATURE");
        } else {
            //NEED TO MAKE A RELOAD SYSTEM FOR GAME DO NOT FORGET
            ggprint16(&f, 16, 0x00ff0000, "3350 - TopGun");
            ggprint16(&i, 16, 0x00ff0000, "Health Points Left: %i", hp);
            ggprint8b(&d, 16, 0x00ffff00, "Number of Enemies Left: %i", nasteroids);
            ggprint8b(&s, 16, 0x00ffffff, "Press F1 To Enter HelpScreen");
        }
    }
    //This will be my Function that will detect collision and will coincide with taking damage aswell

    void collision_detection(int sizeasteroids, int newshape) 
    {
        float theta;
        if (newshape == 1) {
            glBegin(GL_TRIANGLES);

            glVertex2f(sizeasteroids, sizeasteroids);
            glVertex2f(sizeasteroids*3, sizeasteroids);
            glVertex2f(sizeasteroids*2, sizeasteroids*3);

            glEnd();
        }
        if (newshape == 0) {
           glBegin(GL_POLYGON);
            for(int i = 0; i < 360; i++) {
                theta = i*3.142/180;

                glVertex2f(sizeasteroids*cos(theta), sizeasteroids*sin(theta));
            }
                glEnd();
        }
        if (newshape == 2) {
            glBegin(GL_QUADS);

            glVertex2f(-sizeasteroids*4, sizeasteroids*3);
            glVertex2f(-sizeasteroids, sizeasteroids*3);
            glVertex2f(-sizeasteroids, sizeasteroids);
            glVertex2f(-sizeasteroids*4, sizeasteroids);

        glEnd();
        }
    }

    //This Function is to create a border to indicate that you have reached my feature mode
    void FeatureMode_Indication(int xres, int yres)
    {

        // Draw a border using a triangle strip
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glColor4f(rand(),rand(), rand(), rand());

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
}
