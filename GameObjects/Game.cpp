/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for setting up and running game.
*/
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
    feature = 0;
};

/* Game Object */
Game::Game(Global &gl) : gl(gl)
{
    players.push_back(Player(gl.xres, gl.yres));
    // starting 10 enemies...
    spawnWave();
}

Player& Game::getMainPlayer()
{
    return players.at(0);
}

void Game::cleanDead()
{
    std::erase_if(enemies, [](Enemy e) {
        return e.health <= 0;
    });

    // std::erase_if(players, [](Player p) {
    //     return p.health <= 0;
    // });

    auto currentTime = std::chrono::steady_clock::now();

    std::erase_if(bullets, [&](Bullet b) {
        auto diff = currentTime - b.createdAt;
        return b.health <= 0 || diff >= std::chrono::seconds(b.lifetime);
    });
}

void Game::spawnWave()
{
    timeWaveReset = std::chrono::steady_clock::now();
    for (int i = 0; i < waveSize; i++)
    {
        enemies.push_back(Enemy(gl.xres, gl.yres));
    }
    waveSize += 5;

}

void Game::reset()
{
    players.clear();
    bullets.clear();
    enemies.clear();

    score = 0;
    waveSize = 10;

    players.push_back(Player(gl.xres, gl.yres));

    spawnWave();
}
