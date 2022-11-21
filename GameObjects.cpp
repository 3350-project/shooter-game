#include <string.h>
#include <cmath>
#include "GameObjects.h"

/* Global Object */
Global::Global()
{
    xres = 1280;
    yres = 720;
    memset(keys, 0, 65536);
    HelpScr = 0;
    dead = 0;
    credits = 0;    
    sound = 0;
    intro = 0;
    weapon = 0;
};

/* Ship Object */
Ship::Ship(Global gl)
{
    pos[0] = (Flt)(gl.xres / 2);
    pos[1] = (Flt)(gl.yres / 2);
    pos[2] = 0.0f;
    VecZero(dir);
    VecZero(vel);
    VecZero(acc);
    angle = 0.0;
    color[0] = color[1] = color[2] = 1.0;
}

/* Bullet Object */
Bullet::Bullet() {}

/* Asteroid Object */
Asteroid::Asteroid()
{
    prev = NULL;
    next = NULL;
}

/* Game Object */
Game::Game(Global &gl) : gl(gl)
{
    this->gl = gl;
    ship = new Ship(gl);
    ahead = NULL;
    barr = new Bullet[MAX_BULLETS];
    nasteroids = 0;
    nbullets = 0;
    mouseThrustOn = false;
    // build 10 asteroids...
    for (int j = 0; j < 10; j++) {
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
        // std::cout << "asteroid" << std::endl;
        // add to front of linked list
        a->next = ahead;
        if (ahead != NULL)
            ahead->prev = a;
        ahead = a;
        ++nasteroids;
    }
    clock_gettime(CLOCK_REALTIME, &bulletTimer);
}

Game::~Game()
{
    delete[] barr;
}
