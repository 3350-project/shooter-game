#include "GameShared.h"

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

/* Game Object */
Game::Game(Global &gl) : gl(gl)
{
    players.push_back(Player(gl.xres, gl.yres));
    mouseThrustOn = false;
    // starting 10 enemies...
    for (int j = 0; j < 10; j++)
    {
        enemies.push_back(Enemy(gl.xres, gl.yres));
    }
    clock_gettime(CLOCK_REALTIME, &bulletTimer);
}

void Game::cleanDead()
{
    std::erase_if(enemies, [](Enemy e) {
        return e.health <= 0;
    });

    std::erase_if(players, [](Player p) {
        return p.health <= 0;
    });
}
