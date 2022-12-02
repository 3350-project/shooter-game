/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for setting up and running game.
*/
#ifndef GAME_H
#define GAME_H

#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/Xlib.h>

class Global {
    public:
        int xres, yres;
        float w;
        float dir;
        float pos[2];
        int HelpScr;
        int credits;
        int sound, soundTestMode;
        int intro;
        int Collision;
        int weapon;
        int feature;
        char keys[65536];
        bool paused{false};
        unsigned int dead;	
        GLuint ferretTex, zombieTex;
        Global();
};

class Game {
    public:
        std::vector<Player> players;
        std::vector<Enemy> enemies;
        std::vector<Bullet> bullets;
        Global &gl;
        struct timespec bulletTimer;
        struct timespec mouseThrustTimer;
        const int MOVE_SPEED = 10;
        int score = 0;
        int waveSize = 10;
        bool invincibilityFrames = true;
        int sizeasteroids = 40;
        int newshape = 0;
        int flashred = 20;
        int wavenum = 0;
    public:
        Game(Global&);
        void cleanDead();
        Player& getMainPlayer();
        void reset();
        void spawnWave();
        std::chrono::time_point<std::chrono::steady_clock> timeWaveReset;
};
/*
class Image
{
    public:
        int width, height;
        unsigned char *data;
        Image(const char*);
};
*/
#endif
