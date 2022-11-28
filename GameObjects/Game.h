#ifndef GAME_H
#define GAME_H

class Global {
    public:
        int xres, yres, HelpScr, credits, sound, soundTestMode, intro, Collision, weapon;
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
        bool mouseThrustOn;
    public:
        Game(Global&);
        void cleanDead();
};

#endif