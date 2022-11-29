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

class Global {
    public:
        int xres, yres;
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
        static const int MAX_BULLETS = 30;
        const int BULLET_DURATION = 1;
        const int MOVE_SPEED = 10;
        int score = 0;
        int waveSize = 10;
        bool invincibilityFrames = true;
        int sizeasteroids = 40;
        int newshape = 0;
        int flashred = 20;
    public:
        Game(Global&);
        void cleanDead();
        Player& getMainPlayer();
        void reset();
        void spawnWave();
        std::chrono::time_point<std::chrono::steady_clock> timeWaveReset;
};

#endif
