//CMPS3350 - Software Engineering
//Fall 2022 - Team-3 Project
//Program modified by: Axel Arcos, Ryan Gordon, Steven Nez, 
//                     Rodolfo Velasquez, Reid Wyatt

//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2021
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <math.h>
#include "log.h"
#include "fonts.h"

// Game includes
#include "GameObjects/GameShared.h"
#include "Graphics.h"

//Personal file functions
#include "rwyatt.h"
#include "snez.h"
#include "aarcosavalos.h"
#include "rvelasquez.h"
#include "rgordon.h"
//Adding this for the mean time

#define rnd() (((float)rand())/(float)RAND_MAX)
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
//Image ferret("ferret.ppm"), zombie("zombie.ppm");
// Personal class instance
RWyatt rw;
RW::WeaponHandler wh;

#ifdef AUDIO 
SoundDevice * mysounddevice = SoundDevice::get();

SoundSource mySpeaker1;
SoundSource mySpeaker2;
SoundSource mySpeaker3;
SoundSource mySpeaker4;
SoundSource mySpeaker5;
SoundSource mySpeaker6;
SoundSource mySpeaker7;
SoundSource mySpeaker8;
SoundSource mySpeaker9;
SoundSource mySpeaker10;
SoundSource mySpeaker11;
SoundSource gunSpeaker;

uint32_t /*Spearker1*/pistol = SoundBuffer::get()->addSoundEffect("./soundFiles/9mm_pistol.wav");
uint32_t /*Spearker2*/shotgun = SoundBuffer::get()->addSoundEffect("./soundFiles/shotgun.wav");
uint32_t /*Spearker3*/sniper = SoundBuffer::get()->addSoundEffect("./soundFiles/sniper_rifle.wav");
uint32_t /*Spearker4*/machineGun = SoundBuffer::get()->addSoundEffect("./soundFiles/50-cal-mg.wav"); 
uint32_t /*Spearker5*/explosion = SoundBuffer::get()->addSoundEffect("./soundFiles/explode.wav");
uint32_t /*Spearker6*/sucess = SoundBuffer::get()->addSoundEffect("./soundFiles/success.wav");
uint32_t /*Spearker7*/failure = SoundBuffer::get()->addSoundEffect("./soundFiles/gameOver.wav");
uint32_t /*Spearker8*/dying = SoundBuffer::get()->addSoundEffect("./soundFiles/dying.wav");
uint32_t /*Spearker9*/dawn = SoundBuffer::get()->addSoundEffect("./soundFiles/DawnFinal24hrs.wav");
uint32_t /*Spearker10*/reload = SoundBuffer::get()->addSoundEffect("./soundFiles/reload.wav");
uint32_t /*Spearker11*/rifle = SoundBuffer::get()->addSoundEffect("./soundFiles/rifle.wav");
uint32_t gunBuffer;

#endif

//function prototypes
void init_opengl(void);
void init_textures(void);
    // File format:
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
void DrawSquare();

int shape = 1;
//==========================================================================
// M A I N
//==========================================================================
int main()
{
    init_opengl();
    init_textures();
    clock_gettime(CLOCK_REALTIME, &timePause);
    clock_gettime(CLOCK_REALTIME, &timeStart);
    x11.set_mouse_position(100,100);

    // start with pistol
    wh.setActiveWeapon(1);
    gunSpeaker = mySpeaker1;
    gunBuffer = pistol;

    int done=0;
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
//        aarcosavalos::intro_screen();
        render();
        x11.swapBuffers();
    }
    cleanup_fonts();
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

void init_textures(void)
{
/*	
     // Enable texture in Opengl
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &gl.ferretTex);
    int w = ferret.width;
    int h = ferret.height;
    glBindTexture(GL_TEXTURE_2D, gl.ferretTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB,
            GL_UNSIGNED_BYTE, ferret.data);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &gl.zombieTex);
    w = zombie.width;
    h = zombie.height;
    glBindTexture(GL_TEXTURE_2D, gl.zombieTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB,
            GL_UNSIGNED_BYTE, zombie.data);
    glBindTexture(GL_TEXTURE_2D, 0);
*/    
}
void normalize2d(Vector3& v)
{
    float len = v.x * v.x + v.y * v.y;
    if (len == 0.0f) {
        v.x = 1.0;
        v.y = 0.0;
        return;
    }
    len = 1.0f / sqrt(len);
    v.x *= len;
    v.y *= len;
}

void check_mouse(XEvent *e)
{
    if (gl.paused) {
        return;
    }
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 100;
    static int savey = 100;

    //std::cout << "m" << std::endl << std::flush;
    if (e->type == ButtonRelease) {
        return;
    }
    if (e->type == ButtonPress) {
        if (e->xbutton.button==1) {
            //Left button is down
        }
        if (e->xbutton.button==3) {
            //Right button is down
        }
    }

    if (savex != e->xbutton.x || savey != e->xbutton.y) {
        Player &mainPlayer = g.getMainPlayer();
        //Mouse moved
        int dx = mainPlayer.position.x - e->xbutton.x;
        int dy = mainPlayer.position.y - (gl.yres - e->xbutton.y);

        double rad = atan2((double)dy, (double)dx);
        double deg = rad * 180.f / M_PI;
        mainPlayer.setRotation(deg+90.0f);

        // x11.set_mouse_position(100,100);
        savex = e->xbutton.x;
        savey = e->xbutton.y;
    }
}

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
#ifdef AUDIO
            delete mysounddevice;
#endif
            return 1;
        case XK_F4:
#ifdef AUDIO
            gl.sound = managed_state_sound(gl.sound);
            mySpeaker9.Play(dawn);
#endif
            break;
        case XK_c:
            gl.credits = managed_state_credits(gl.credits);
            break;
        case XK_x:
            g.newshape += 1;
            if(g.newshape == 3 ) {
                g.newshape = 0;
            }
            break;
/*
        case XK_F3:
            gl.weapon = rgordon::manage_state(gl.weapon);
            break;
*/
	case XK_t:
            g.spawnWave();
            break;
	case XK_f:
	    gl.picture = aarcosavalos::manage_state(gl.picture);
	    break;
        case XK_Down:
            break;
        case XK_i:
            gl.intro = aarcosavalos::manage_state(gl.intro);
        case XK_equal:
            break;
        case XK_minus:
            break;
        case XK_r:
            wh.getActiveWeapon().reloadWeapon();
#ifdef AUDIO
            if (gl.sound)
                mySpeaker10.Play(reload);    
#endif
            break;
        case XK_n:
            if (rw.getPromptSaveScore()) {
                rw.switchPromptSaveScore();
                RWyatt::pauseScreen(gl.paused);
                break;
            }
            g.sizeasteroids += 5;
            break;
        case XK_m:
            if(g.sizeasteroids < 5) {
                g.sizeasteroids = 0;
            } else {
                g.sizeasteroids -= 5;
            }
            break;
        case XK_F1:
            gl.HelpScr = snez::manage_stateF1(gl.HelpScr);
            break;
        case XK_p:
            RWyatt::pauseScreen(gl.paused);
            // unlocks and shows cursor
            // x11.show_mouse_cursor(gl.paused);
            break;
        case XK_F2:
            gl.Collision = snez::manage_stateF2(gl.Collision);
            break;
	case XK_F3:
            gl.feature = aarcosavalos::manage_state(gl.feature);    
            break;
        case XK_g:
            gl.dead = 1;
            break;
        case XK_l:
            g.wavenum += 1;
            if (g.wavenum == 5)
                g.wavenum = 1;
            break;
        case XK_y:
            if (rw.getPromptSaveScore()) {
                rw.savePlayerData();
                rw.switchPromptSaveScore();
                RWyatt::pauseScreen(gl.paused);
                break;
            }
            // if player chose to play again make it 0 (false)
            gl.dead = 0;
            gl.paused = false;
            g.sizeasteroids = 40;
            g.newshape = 0;
            g.reset();
            g.wavenum = 1;
            
            break;
        case XK_F9:
            rw.switchPromptSaveScore();
            RWyatt::pauseScreen(gl.paused);
            break;
        // Weapon Swap Keys
        case XK_1:
	        shape = 1;	    
            wh.setActiveWeapon(1);
            gunSpeaker = mySpeaker1;
            gunBuffer = pistol;
            break;
        case XK_2:
	        shape = 2;	    
            wh.setActiveWeapon(2);
            gunSpeaker = mySpeaker11;
            gunBuffer = rifle;
            break;
        case XK_3:
	        shape = 3;	    
            wh.setActiveWeapon(3);
            gunSpeaker = mySpeaker2;
            gunBuffer = shotgun;
            break;
        case XK_4:
	        shape = 4;	    
            wh.setActiveWeapon(4);
            gunSpeaker = mySpeaker3;
            gunBuffer = sniper;
            break;
        case XK_5:
	        shape = 5;	  
            wh.setActiveWeapon(5);
            gunSpeaker = mySpeaker4;
            gunBuffer = machineGun;
            break;
    }
    return 0;
}

void physics()
{
    if (gl.paused || gl.HelpScr || gl.dead || gl.intro) {

        return;
    }
    float d0,d1,dist, d2, d3, dist2;
    //Update ship position---------------- trying to make asteroids close into ship

    // start wave with invincibility
    auto currentTime = std::chrono::steady_clock::now();
    auto diff = currentTime - g.timeWaveReset;
    if (diff >= std::chrono::seconds(3)) {
        g.invincibilityFrames = false;
    } else {
        g.invincibilityFrames = true;
    }

    // Player updates
    for (Player& p : g.players) {
        p.updatePosition();

        //Check for collision with window edges
        if (p.position.x < 0.0) {
            p.position.x += (float)gl.xres;
        }
        else if (p.position.x > (float)gl.xres) {
            p.position.x -= (float)gl.xres;
        }
        else if (p.position.y < 0.0) {
            p.position.y += (float)gl.yres;
        }
        else if (p.position.y > (float)gl.yres) {
            p.position.y -= (float)gl.yres;
        }
    }

    // Bullet updates
    for (Bullet& b : g.bullets) {
        b.updatePosition();

        // //Check for collision with window edges
        // if (b.position.x < 0.0) {
        //     b.position.x += (float)gl.xres;
        // }
        // else if (b.position.x > (float)gl.xres) {
        //     b.position.x -= (float)gl.xres;
        // }
        // else if (b.position.y < 0.0) {
        //     b.position.y += (float)gl.yres;
        // }
        // else if (b.position.y > (float)gl.yres) {
        //     b.position.y -= (float)gl.yres;
        // }
    }

    // Asteroid updates
    for (Enemy& e : g.enemies) {
        e.updatePosition();

        if (e.position.x < -100.0) {
            e.position.x += (float)gl.xres+200;
        }
        else if (e.position.x > (float)gl.xres+100) {
            e.position.x -= (float)gl.xres+200;
        }
        else if (e.position.y < -100.0) {
            e.position.y += (float)gl.yres+200;
        }
        else if (e.position.y > (float)gl.yres+100) {
            e.position.y -= (float)gl.yres+200;
        }
    }

    // Collisions
    for (Enemy& e : g.enemies) {
        for (Bullet& b : g.bullets) {
            d0 = b.position.x - e.position.x;
            d1 = b.position.y - e.position.y;
            dist = d0 * d0 + d1 * d1;

            if (dist < e.colisionRadius * e.colisionRadius &&
                    dist < e.colisionRadius * g.sizeasteroids) {
                std::cout << "Enemy Killed!" << std::endl;

                // delete enemy and bullet
                e.hitHealth();
                b.hitHealth();
                rw.getPlayerData().addToScore();
                rw.getPlayerData().addToEnemiesKilled();
                g.score += 1;
#ifdef AUDIO
                if (gl.sound)
                    mySpeaker5.Play(explosion);
#endif
            }
        }
    }

    for (Enemy& e : g.enemies) {
        for (Player &p : g.players) {
            d2 = p.position.x - e.position.x;
            d3 = p.position.y - e.position.y;
            dist2 = d2 * d2 + d3 * d3;

            if (dist2 < e.colisionRadius * g.sizeasteroids * 2 && p.health > 0) {
                if (!g.invincibilityFrames) {
                    p.hitHealth();
#ifdef AUDIO
                    if (gl.sound)
                        mySpeaker8.Play(dying);
#endif
                }
                e.hitHealth();
                rw.getPlayerData().subtractFromScore();
                g.score -= 1;
                p.velocity.x = 0;
                p.velocity.y = 0;
                g.flashred = 1;
                std::cout<< "Player has collided with enemy!" << std::endl;

#ifdef AUDIO
                if (gl.sound )
                    mySpeaker5.Play(explosion);
#endif
            }
        }
    }

    if (g.getMainPlayer().health <= 0){
        gl.dead = 1;
    
#ifdef AUDIO
    if (gl.sound)
        mySpeaker7.Play(failure);
#endif
    }
    //---------------------------------------------------
    //check keys pressed now

    Player &mainPlayer = g.getMainPlayer();
    if (gl.keys[XK_a]) {
        mainPlayer.moveLeft(g.MOVE_SPEED);
    }
    if (gl.keys[XK_d]) {
        mainPlayer.moveRight(g.MOVE_SPEED);
    }
    if (gl.keys[XK_w]) {
        mainPlayer.moveUp(g.MOVE_SPEED);
    }
    if (gl.keys[XK_s]) {
        mainPlayer.moveDown(g.MOVE_SPEED);
    }
    if (gl.keys[XK_space]) {
        //a little time between each bullet
        struct timespec bt;
        clock_gettime(CLOCK_REALTIME, &bt);
        double ts = timeDiff(&g.bulletTimer, &bt);
        if (ts >= wh.getActiveWeapon().getFireRate()) {
            timeCopy(&g.bulletTimer, &bt);
            //shoot a bullet...
            auto newBullets = wh.fireActiveWeapon(g.getMainPlayer());
            for (Bullet b : newBullets) {
                g.bullets.push_back(b);
            }
            rw.getPlayerData().addToShotsFired();
#ifdef AUDIO
            if (gl.sound && wh.getActiveWeapon().getCurrentMagizine() > 0)
                gunSpeaker.Play(gunBuffer);
#endif
        }
    }
    g.cleanDead();

    if (g.enemies.empty()) {
#ifdef AUDIO
        if (gl.sound)
            mySpeaker6.Play(sucess);
#endif
        g.sizeasteroids += 5;
        g.spawnWave();

        std::cout << "Now Going into Wave " << g.wavenum << std::endl;

        for (Enemy& e : g.enemies) {
            if (g.wavenum == 2) {
                e.velocity.x *= 1.05;
                e.velocity.y *= 1.05;
            }
            if (g.wavenum == 3) {
                e.velocity.x *= 1.1;
                e.velocity.y *= 1.1;

            }
            if (g.wavenum >= 4) {
                e.velocity.x *= 1.15;
                e.velocity.y *= 1.15;
            }
        }
#ifdef AUDIO
        if (gl.sound)
            mySpeaker9.Play(dawn);
#endif
   }
}

void render()
{
    Player &mainPlayer = g.getMainPlayer();
    snez::Featuremode(gl.xres, gl.yres, gl.Collision, mainPlayer.health, g.enemies.size());
    //-------------------------------------------------------------------------

    // Draw score
    rw.drawScore(gl.xres, gl.yres);

    // Draw weapon UI
    RWyatt::drawWeaponUI(gl.xres, gl.yres, wh);


    //Draw the player
    if (g.flashred < 20) {
        glColor3f(1.0f, 0.0, 0.0);
        g.flashred += 1;

    } else {
        g.flashred = 20;
        glColor3f(mainPlayer.color.x, 
                mainPlayer.color.y,
                mainPlayer.color.z);
    }
    glPushMatrix();
    glTranslatef(mainPlayer.position.x, mainPlayer.position.y, mainPlayer.position.z);
    glRotatef(mainPlayer.rotation, 0.0f, 0.0f, 1.0f);
    if (shape == 1){aarcosavalos::change_shape(1);}
    if (shape == 2){aarcosavalos::change_shape(2);}
    if (shape == 3){aarcosavalos::change_shape(3);}
    if (shape == 4){aarcosavalos::change_shape(4);}
    if (shape == 5){aarcosavalos::change_shape(5);}
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
    glPopMatrix();

    //Draw the asteroids
    for (Enemy e : g.enemies) {
        if (g.wavenum < 2)
            glColor3f(1.0f, 1.0f, 1.0f);
        if (g.wavenum == 2)
            glColor4f(1.0f,1.0f, 0.0f, 0.0f);
        if (g.wavenum == 3)
            glColor3f(1.0f, 0.5f, 0.0f);
        if(g.wavenum >=4)
            glColor4f(1.0f,0.0f,0.0f,0.0f);
        glPushMatrix();
        glTranslatef(e.position.x, e.position.y, e.position.z);
        glRotatef(e.rotation, 0.0f, 0.0f, 1.0f);
        //Changing Asteroids into circles
        snez::collision_detection(g.sizeasteroids, g.newshape);
        glPopMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
    }

    //Draw the bullets
    for (Bullet b : g.bullets) {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POINTS);
        glVertex2f(b.position.x,      b.position.y);
        glVertex2f(b.position.x-1.0f, b.position.y);
        glVertex2f(b.position.x+1.0f, b.position.y);
        glVertex2f(b.position.x,      b.position.y-1.0f);
        glVertex2f(b.position.x,      b.position.y+1.0f);
        glColor3f(0.8, 0.8, 0.8);
        glVertex2f(b.position.x-1.0f, b.position.y-1.0f);
        glVertex2f(b.position.x-1.0f, b.position.y+1.0f);
        glVertex2f(b.position.x+1.0f, b.position.y-1.0f);
        glVertex2f(b.position.x+1.0f, b.position.y+1.0f);
        glEnd();
    }

    while(gl.HelpScr) {
        snez::Show_HelpScr(gl.xres, gl.yres);
        gl.credits = 0;
        gl.dead = false;
        gl.paused = false;
        return;
    }
    if (rw.getPromptSaveScore()) {
        rw.drawPromptSaveScore(gl.xres, gl.yres);
    }
    if(gl.dead || mainPlayer.health == 0) {
        aarcosavalos::finish_game(gl.xres, gl.yres);
        gl.paused = true;
        return;
    }
    if(gl.credits || g.score == 100) {
	    gl.paused = true;
        show_credits(gl.xres, gl.yres);
        return;
    }
    if(gl.Collision) {
        snez::FeatureMode_Indication(gl.xres,gl.yres);
        return;
    }
#ifdef AUDIO
    if(gl.sound) {
        sound(gl.xres, gl.yres);
        return;
    }
#endif
    /*
    if(gl.intro) {
        rgordon::intro(gl.xres, gl.yres);
        return;
    }
    */
    if(gl.weapon) {
        rgordon::weapon(gl.xres, gl.yres);
        return;
    }
    if(gl.feature) {
        aarcosavalos::Feature_mode(gl.xres, gl.yres);
        return;
    }
    if (gl.intro==1) {
        aarcosavalos::intro_screen(gl.xres, gl.yres);
        return;
    } 
    if (gl.picture)
    {
	DrawSquare();
	return;
    }
}

void DrawSquare()
{
    Rect t;
    t.bot = gl.yres/2;
    t.left = gl.xres/2.5;
    t.center = 0;
    // Draw box
    glPushMatrix();
    glColor3ub(0, 0, 0);
    //glTranslatef(gl.pos[0], gl.pos[1], 0.0f);
    glBegin(GL_QUADS);   
       glVertex2f(-gl.xres, -gl.yres);
       glVertex2f(-gl.xres,  gl.yres);
       glVertex2f( gl.xres,  gl.yres);
       glVertex2f( gl.xres, -gl.yres);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnd();
    glPopMatrix();
    ggprint16(&t, 16, 0x00ffffff, "IF YOU ARE READING THIS, IT MEANS");
    ggprint16(&t, 16, 0x00ffffff, "I WAS NOT ABLE TO ADD A PICTURE OF A FERRET");
    ggprint16(&t, 16, 0x00ffffff, "");
    ggprint16(&t, 16, 0x00ffffff, "");
    ggprint16(&t, 16, 0x00ffffff, "");
    ggprint16(&t, 16, 0x00ffffff, "                   ( ' - ' ) ");
	
}

