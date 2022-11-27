//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2021
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <math.h>
#include "log.h"
#include "fonts.h"

// Game includes
#include "GameObjects.h"
#include "Graphics.h"

//Personal file functions
#include "rwyatt.h"
#include "snez.h"
#include "aarcosavalos.h"
#include "rvelasquez.h"
#include "rgordon.h"

//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------

// Game object class instances
Global gl;
Game g = Game(gl);
X11_wrapper x11(1280, 720, gl);

// Personal class instance
RWyatt rw;

//function prototypes
void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
void next_wave(int);

int wave = 10;
int MAX_WAVE = 60;
//==========================================================================
// M A I N
//==========================================================================
int main(int argc, char* argv[])
{
    logOpen();
    init_opengl();
    srand(time(NULL));
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    x11.set_mouse_position(100,100);
    int done=0;
    rw.set_network_config(argc, argv);
    while (!done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            check_mouse(&e);
            done = check_keys(&e);
        }
        clock_gettime(CLOCK_REALTIME, &timeCurrent);
        timeSpan = timeDiff(&timeStart, &timeCurrent);
        timeCopy(&timeStart, &timeCurrent);
        physicsCountdown += timeSpan;
        while (physicsCountdown >= physicsRate) {
            physics();
            physicsCountdown -= physicsRate;
        }
        render();
        x11.swapBuffers();
    }
    cleanup_fonts();
    logClose();
    return 0;
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, gl.xres, gl.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
}

void normalize2d(Vec v)
{
    Flt len = v[0]*v[0] + v[1]*v[1];
    if (len == 0.0f) {
        v[0] = 1.0;
        v[1] = 0.0;
        return;
    }
    len = 1.0f / sqrt(len);
    v[0] *= len;
    v[1] *= len;
}

void check_mouse(XEvent *e)
{
    if (gl.paused) {
        return;
    }
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
    //
    static int ct=0;
    //std::cout << "m" << std::endl << std::flush;
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button is down
            //a little time between each bullet
            struct timespec bt;
            clock_gettime(CLOCK_REALTIME, &bt);
            double ts = timeDiff(&g.bulletTimer, &bt);
            if (ts > 0.1) {
                timeCopy(&g.bulletTimer, &bt);
                //shoot a bullet...
                if (g.nbullets < MAX_BULLETS) {
                    Bullet *b = &g.barr[g.nbullets];
                    timeCopy(&b->time, &bt);
                    b->pos[0] = g.ship->pos[0];
                    b->pos[1] = g.ship->pos[1];
                    b->vel[0] = g.ship->vel[0];
                    b->vel[1] = g.ship->vel[1];
                    //convert ship angle to radians
                    Flt rad = ((g.ship->angle+90.0) / 360.0f) * PI * 2.0;
                    //convert angle to a vector
                    Flt xdir = cos(rad);
                    Flt ydir = sin(rad);
                    b->pos[0] += xdir*20.0f;
                    b->pos[1] += ydir*20.0f;
                    b->vel[0] += xdir*6.0f + rnd()*0.1;
                    b->vel[1] += ydir*6.0f + rnd()*0.1;
                    b->color[0] = 1.0f;
                    b->color[1] = 1.0f;
                    b->color[2] = 1.0f;
                    ++g.nbullets;
                }
            }
        }
        if (e->xbutton.button==3) {
            //Right button is down
        }
    }
    //keys[XK_Up] = 0;
    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        //Mouse moved
        int xdiff = savex - e->xbutton.x;
        int ydiff = savey - e->xbutton.y;
        if (++ct < 10)
            return;     
        //std::cout << "savex: " << savex << std::endl << std::flush;
        //std::cout << "e->xbutton.x: " << e->xbutton.x << std::endl <<
        //std::flush;
        if (xdiff > 0) {
            //std::cout << "xdiff: " << xdiff << std::endl << std::flush;
            g.ship->angle += 0.05f * (float)xdiff;
            if (g.ship->angle >= 360.0f)
                g.ship->angle -= 360.0f;
        }
        else if (xdiff < 0) {
            //std::cout << "xdiff: " << xdiff << std::endl << std::flush;
            g.ship->angle += 0.05f * (float)xdiff;
            if (g.ship->angle < 0.0f)
                g.ship->angle += 360.0f;
        }
        if (ydiff > 0) {
            //apply thrust
            //convert ship angle to radians
            Flt rad = ((g.ship->angle+90.0) / 360.0f) * PI * 2.0;
            //convert angle to a vector
            Flt xdir = cos(rad);
            Flt ydir = sin(rad);
            g.ship->vel[0] += xdir * (float)ydiff * 0.01f;
            g.ship->vel[1] += ydir * (float)ydiff * 0.01f;
            Flt speed = sqrt(g.ship->vel[0]*g.ship->vel[0]+
                    g.ship->vel[1]*g.ship->vel[1]);
            if (speed > 10.0f) {
                speed = 10.0f;
                normalize2d(g.ship->vel);
                g.ship->vel[0] *= speed;
                g.ship->vel[1] *= speed;
            }
            g.mouseThrustOn = true;
            clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
        }
        x11.set_mouse_position(100,100);
        savex = 100;
        savey = 100;
    }
}
int sizeasteroids = 40;
int newshape = 0;
int check_keys(XEvent *e)
{
    static int shift=0;
    if (e->type != KeyRelease && e->type != KeyPress) {
        //not a keyboard event
        return 0;
    }
    int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
    //Log("key: %i\n", key);
    if (e->type == KeyRelease) {
        gl.keys[key] = 0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift = 0;
        return 0;
    }
    if (e->type == KeyPress) {
        //std::cout << "press" << std::endl;
        gl.keys[key]=1;
        if (key == XK_Shift_L || key == XK_Shift_R) {
            shift = 1;
            return 0;
        }
    }
    (void)shift;
    switch (key) {
        case XK_Escape:
            return 1;
        case XK_f:
            break;
        case XK_s:
			gl.sound = managed_state_sound(gl.sound);
            break;
        case XK_c:
            gl.credits = managed_state_credits(gl.credits);
            break;
        case XK_x:
             newshape +=1;
             if(newshape == 3 ) {
                newshape = 0;
            }
            break;          
        case XK_F3:
            gl.weapon = rgordon::manage_state(gl.weapon);
            break;
        case XK_t:
	    next_wave(wave);
            wave = wave + 5;
            break;
        case XK_Down:
            break;
        case XK_equal:
            break;
        case XK_minus:
            break;
        case XK_r:
            break;
        case XK_n:
            sizeasteroids += 5;
            break;
        case XK_m:
            if(sizeasteroids < 5) {
            sizeasteroids = 0;
            } else {
                sizeasteroids -= 5;
            }
            break;
        case XK_i:
            gl.intro = rgordon::manage_state(gl.intro);
            break;
        case XK_F1:
            gl.HelpScr = snez::manage_stateF1(gl.HelpScr);
            break;
        case XK_p:
            RWyatt::pause_screen(gl.paused);
            // unlocks and shows cursor
            x11.show_mouse_cursor(gl.paused);
            break;
        case XK_F7:
            !rw.networked() ? rw.start_networking() : rw.stop_networking();
            break;
        case XK_F2:
            gl.Collision = snez::manage_stateF2(gl.Collision);
            break;
        case XK_g:
            gl.dead = 1;
            break;
        case XK_y:
            // if player chose to play again make it 0 (false)
            gl.dead = 0;
            break;
    }
    return 0;
}

void deleteAsteroid(Game *g, Asteroid *node)
{
    //Remove a node from doubly-linked list
    //Must look at 4 special cases below.
    if (node->prev == NULL) {
        if (node->next == NULL) {
            //only 1 item in list.
            g->ahead = NULL;
        } else {
            //at beginning of list.
            node->next->prev = NULL;
            g->ahead = node->next;
        }
    } else {
        if (node->next == NULL) {
            //at end of list.
            node->prev->next = NULL;
        } else {
            //in middle of list.
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
    }
    delete node;
    node = NULL;
}

void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
    //build ta from a
    ta->nverts = 8;
    ta->radius = a->radius / 2.0;
    Flt r2 = ta->radius / 2.0;
    Flt angle = 0.0f;
    Flt inc = (PI * 2.0) / (Flt)ta->nverts;
    for (int i=0; i<ta->nverts; i++) {
        ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
        ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
        angle += inc;
    }
    ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
    ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
    ta->pos[2] = 0.0f;
    ta->angle = 0.0;
    ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
    ta->color[0] = 0.8;
    ta->color[1] = 0.8;
    ta->color[2] = 0.7;
    ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
    ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
    //std::cout << "frag" << std::endl;
}
int hp = 3;
int flashred = 0;
void physics()
{
    if (gl.paused|| gl.HelpScr || gl.dead) {
        return;
    }
    Flt d0,d1,dist; //d2, d3, dist2;
    //Update ship position---------------- trying to make asteroids close into ship
    g.ship->pos[0] += g.ship->vel[0];
    g.ship->pos[1] += g.ship->vel[1];
    //Check for collision with window edges
    if (g.ship->pos[0] < 0.0) {
        g.ship->pos[0] += (float)gl.xres;
    }
    else if (g.ship->pos[0] > (float)gl.xres) {
        g.ship->pos[0] -= (float)gl.xres;
    }
    else if (g.ship->pos[1] < 0.0) {
        g.ship->pos[1] += (float)gl.yres;
    }
    else if (g.ship->pos[1] > (float)gl.yres) {
        g.ship->pos[1] -= (float)gl.yres;
    }
    //
    //
    //Update bullet positions
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    int i = 0;
    while (i < g.nbullets) {
        Bullet *b = &g.barr[i];
        //How long has bullet been alive?
        double ts = timeDiff(&b->time, &bt);
        if (ts > 1.0) {
            //time to delete the bullet.
            memcpy(&g.barr[i], &g.barr[g.nbullets-1],
                    sizeof(Bullet));
            g.nbullets--;
            //do not increment i.
            continue;
        }
        //move the bullet
        b->pos[0] += b->vel[0];
        b->pos[1] += b->vel[1];
        //Check for collision with window edges
        if (b->pos[0] < 0.0) {
            b->pos[0] += (float)gl.xres;
        }
        else if (b->pos[0] > (float)gl.xres) {
            b->pos[0] -= (float)gl.xres;
        }
        else if (b->pos[1] < 0.0) {
            b->pos[1] += (float)gl.yres;
        }
        else if (b->pos[1] > (float)gl.yres) {
            b->pos[1] -= (float)gl.yres;
        }
        ++i;
    }
    //
    //Update asteroid positions
    Asteroid *a = g.ahead;
    while (a) {
        //Asteroid Movement
    	a->pos[0] += a->vel[0];
    	a->pos[1] += a->vel[1];


        //Check for collision with window edges
        if (a->pos[0] < -100.0) {
            a->pos[0] += (float)gl.xres+200;
        }
        else if (a->pos[0] > (float)gl.xres+100) {
            a->pos[0] -= (float)gl.xres+200;
        }
        else if (a->pos[1] < -100.0) {
            a->pos[1] += (float)gl.yres+200;
        }
        else if (a->pos[1] > (float)gl.yres+100) {
            a->pos[1] -= (float)gl.yres+200;
        }
        a = a->next;
    }
    //
    //Asteroid collision with bullets?
    //If collision detected:
    //     1. delete the bullet
    //     2. break the asteroid into pieces
    //        if asteroid small, delete it
    a = g.ahead;
    while (a) {
        //is there a bullet within its radius?
        int i=0;
        while (i < g.nbullets) {
            Bullet *b = &g.barr[i];
            d0 = b->pos[0] - a->pos[0];
            d1 = b->pos[1] - a->pos[1];
            dist = (d0*d0 + d1*d1);
            if (gl.Collision == 0) {
            //Testing for smaller radius collision
                if (dist < (a->radius*a->radius)) {
                    std::cout << "asteroid hit." << std::endl;
                    //delete the asteroid and bullet
                    Asteroid *savea = a->next;
                    deleteAsteroid(&g, a);
                    a = savea;
                    g.nasteroids--;
                    //delete the bullet...
                    memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
                    g.nbullets--;
                    if (a == NULL)
                        break;
                }
            } else {
               if (dist < (a->radius * 20 )) {
                    std::cout << "asteroid hit." << std::endl;
                    //this asteroid is hit.
                //delete the asteroid and bullet
                Asteroid *savea = a->next;
                deleteAsteroid(&g, a);
                a = savea;
                g.nasteroids--;
                //delete the bullet...
                memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
                g.nbullets--;
                if (a == NULL)
                    break;
                }
        }
            i++;
            }
        if (a == NULL)
            break;
        a = a->next;
    }
       /* while (i < g.nasteroids) {
                    d2 = g.ship->pos[0] - a->pos[0];
                    d3 = g.ship->pos[1] - a->pos[1];
                    dist2 = (d2*d2 + d3*d3);
           if(dist2 < (a->radius * sizeasteroids) && hp != 0) {
                    hp--;
		            g.ship->vel[0] = 0;
		            g.ship->vel[1] = 0;
                    flashred = 1;

                    std::cout<<"Ship has collided with asteroid" << std::endl;
           }else {
                    Asteroid *savea = a->next;
                    deleteAsteroid(&g, a);
                    a = savea;
		            g.nasteroids--;
                    if(hp == 0) {
                        std::cout << "Hp is now at 0" << std::endl;
                            if (hp < 0) {
                                hp = 0;
                            }

                    }
                    if (a == NULL)
                        break;
                }
        }*/
    //---------------------------------------------------
    //check keys pressed now
    if (gl.keys[XK_Left]) {
        g.ship->angle += 4.0;
        if (g.ship->angle >= 360.0f)
            g.ship->angle -= 360.0f;
    }
    if (gl.keys[XK_Right]) {
        g.ship->angle -= 4.0;
        if (g.ship->angle < 0.0f)
            g.ship->angle += 360.0f;
    }
    if (gl.keys[XK_Up]) {
        //apply thrust
        //convert ship angle to radians
        Flt rad = ((g.ship->angle+90.0) / 360.0f) * PI * 2.0;
        //convert angle to a vector
        Flt xdir = cos(rad);
        Flt ydir = sin(rad);
        g.ship->vel[0] += xdir*0.1f;
        g.ship->vel[1] += ydir*0.1f;
        Flt speed = sqrt(g.ship->vel[0]*g.ship->vel[0]+
                g.ship->vel[1]*g.ship->vel[1]);
        if (speed > 5.0f) {
            speed = 5.0f;
            normalize2d(g.ship->vel);
            g.ship->vel[0] *= speed;
            g.ship->vel[1] *= speed;
        }
    }
    if (gl.keys[XK_space]) {
        //a little time between each bullet
        struct timespec bt;
        clock_gettime(CLOCK_REALTIME, &bt);
        double ts = timeDiff(&g.bulletTimer, &bt);
        if (ts > 0.5) {
            timeCopy(&g.bulletTimer, &bt);
            if (g.nbullets < MAX_BULLETS) {
                //shoot a bullet...
                //Bullet *b = new Bullet;
                Bullet *b = &g.barr[g.nbullets];
                timeCopy(&b->time, &bt);
                b->pos[0] = g.ship->pos[0];
                b->pos[1] = g.ship->pos[1];
                b->vel[0] = g.ship->vel[0];
                b->vel[1] = g.ship->vel[1];
                //convert ship angle to radians
                Flt rad = ((g.ship->angle+90.0) / 360.0f) * PI * 2.0;
                //convert angle to a vector
                Flt xdir = cos(rad);
                Flt ydir = sin(rad);
                b->pos[0] += xdir*20.0f;
                b->pos[1] += ydir*20.0f;
                b->vel[0] += xdir*6.0f + rnd()*0.1;
                b->vel[1] += ydir*6.0f + rnd()*0.1;
                b->color[0] = 1.0f;
                b->color[1] = 1.0f;
                b->color[2] = 1.0f;
                g.nbullets++;
            }
       }
    }
    if (g.mouseThrustOn) {
        //should thrust be turned off
        struct timespec mtt;
        clock_gettime(CLOCK_REALTIME, &mtt);
        double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
        //std::cout << "tdif: " << tdif << std::endl;
        if (tdif < -0.3)
            g.mouseThrustOn = false;
    }
}

void render()
{
    snez::Featuremode(gl.xres, gl.yres, gl.Collision, g.nbullets, g.nasteroids);
    //-------------------------------------------------------------------------
    //Draw the ship
    if (flashred == 1) {
        glColor3f(1.0f, 0.0, 0.0);
        flashred = 0;
    } else {
    glColor3fv(g.ship->color);
    }
    glPushMatrix();
    glTranslatef(g.ship->pos[0], g.ship->pos[1], g.ship->pos[2]);
    glRotatef(g.ship->angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-12.0f, -10.0f);
    glVertex2f(  0.0f,  20.0f);
    glVertex2f(  0.0f,  -6.0f);
    glVertex2f(  0.0f,  -6.0f);
    glVertex2f(  0.0f,  20.0f);
    glVertex2f( 12.0f, -10.0f);
    glEnd();
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();
    //Might not want the thrust so I commented it out
    
    if (gl.keys[XK_space]) {
        int i;
        //draw thrust
        Flt rad = ((g.ship->angle - 90.0) / 360.0f) * PI * 2.0;
        //convert angle to a vector
        Flt xdir = cos(rad);
        Flt ydir = sin(rad);
        Flt xs,ys,xe,ye,r;
        glBegin(GL_LINES);
        for (i=0; i<16; i++) {
            xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
            ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
            r = rnd()*5.0+30.0;
            xe = -xdir * r + rnd() * 18.0 - 9.0;
            ye = -ydir * r + rnd() * 18.0 - 9.0;
            glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
            glVertex2f(g.ship->pos[0]+xs,g.ship->pos[1]+ys);
            glVertex2f(g.ship->pos[0]+xe,g.ship->pos[1]+ye);
        }
        glEnd();
    }
    
    //-------------------------------------------------------------------------
    //Draw the asteroids
    {
        if (!gl.Collision) {
            Asteroid *a = g.ahead;
            while (a) {
                //Log("draw asteroid...\n");
                glColor3fv(a->color);
                glPushMatrix();
                glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
                glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
                //float theta;
                //glBegin(GL_POLYGON);
                glBegin(GL_LINE_LOOP);
                //Log("%i verts\n",a->nverts);
                for (int j=0; j<a->nverts; j++) {
                    glVertex2f(a->vert[j][0], a->vert[j][1]);
                }
                glEnd();
                glPopMatrix();
                glColor3f(1.0f, 0.0f, 0.0f);
                a = a->next;
            }
        }else {
            Asteroid *a = g.ahead;
            while (a) {
                glColor3f(rnd(), rnd(),rnd());
                glPushMatrix();
                glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
                glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
                //Changing Asteroids into circles
                snez::collision_detection(sizeasteroids, newshape);
                glPopMatrix();
                glColor3f(1.0f, 0.0f, 0.0f);
                a = a->next;
            }
        }
	if (g.nasteroids == 0)
        {
           if (wave >= MAX_WAVE)
               wave = MAX_WAVE;
           else
               wave = wave + 5;
           next_wave(wave);
        }
    }
    //-------------------------------------------------------------------------
    //Draw the bullets
    for (int i=0; i<g.nbullets; i++) {
        Bullet *b = &g.barr[i];
        //Log("draw bullet...\n");
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2f(b->pos[0],      b->pos[1]);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]);
        glVertex2f(b->pos[0],      b->pos[1]-1.0f);
        glVertex2f(b->pos[0],      b->pos[1]+1.0f);
        glColor3f(0.8, 0.8, 0.8);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
        glEnd();
    }
    while(gl.HelpScr){
        snez::Show_HelpScr(gl.xres, gl.yres);
        gl.credits = 0;
        gl.dead = false;
        gl.paused = false;
        return;
    }
    if(gl.dead == 1 || hp == 0){
        aarcosavalos::finish_game(gl.xres, gl.yres);
	gl.paused = true;
        return;
    }
    if(gl.credits){
        show_credits(gl.xres, gl.yres);
        return;
    }
    if(gl.Collision) {
	snez::FeatureMode_Indication(gl.xres,gl.yres);
	return;
    }
    if(gl.sound) {
        sound(gl.xres, gl.yres);
        return;
    }
    if(gl.intro){
        rgordon::intro(gl.xres, gl.yres);
        return;
    }
    if(gl.weapon){
        rgordon::weapon(gl.xres, gl.yres);
        return;
    }
    
    if (rw.networked()) {
        RWyatt::draw_border(gl.xres, gl.yres);
        rw.draw_networking(gl.xres, gl.yres);
    }
}

void next_wave (int waves)
{
	std::cout << "NEXT WAVE\n";
	 for (int j = 0; j < waves; j++) {
        Asteroid *a = new Asteroid;
        a->nverts = 8;
        a->radius = rnd() * 80.0 + 40.0;
        Flt r2 = a->radius / 2.0;
        Flt angle = 0.0f;
        Flt inc = (PI * 2.0) / (Flt)a->nverts;
        for (int i = 0; i < a->nverts; i++) {
            a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
            a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
            angle += inc;
        }
        a->pos[0] = (Flt)(rand() % gl.xres);
        a->pos[1] = (Flt)(rand() % gl.yres);
        a->pos[2] = 0.0f;
        a->angle = 0.0;
        a->rotate = rnd() * 4.0 - 2.0;
        a->color[0] = 0.8;
        a->color[1] = 0.8;
        a->color[2] = 0.7;
        a->vel[0] = (Flt)(rnd() * 2.0 - 1.0);
        a->vel[1] = (Flt)(rnd() * 2.0 - 1.0);
        // add to front of linked list
        a->next = g.ahead;
        if (g.ahead != NULL)
            g.ahead->prev = a;
        g.ahead = a;
	++g.nasteroids;
    }
}
