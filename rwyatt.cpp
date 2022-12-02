/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for drawing on screen.
*/
#include <iostream>
#include <filesystem>
#include <fstream>
#include <GL/glx.h>
#include "fonts.h"
#include "rwyatt.h"

/*
 * STATIC METHODS
 */
void RWyatt::flipState(bool &state)
{
    state = !state;
}

void RWyatt::pauseScreen(bool &paused) 
{
    RWyatt::flipState(paused);
}

void RWyatt::drawBorder(int xres, int yres)
{
    // draw a border using a triangle strip
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glColor4f(1.0, 1.0, 0.0, 0.3);
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

void RWyatt::drawScore(int xres, int yres, int score)
{
	Rect r;
	r.bot = yres - 100;
	r.left = xres / 2.5;
	r.center = 0;

    ggprint16(&r, 20, 0x00ffffff, "Score: %d", score);
}

/**
 * CLASS METHODS
*/
RWyatt::RWyatt() 
{
    playerData = getSavedPlayerData();
}

/**
 * SCORE METHODS
*/
bool RWyatt::saveFileExists()
{
    return std::filesystem::exists("savadata");
}

bool RWyatt::savePlayerData()
{
    std::ofstream outf {"savedata"};

    if (!outf) {
        std::cerr << "RWyatt::savePlayerData\n";
        return false;
    }
    
    outf << playerData.getScore() << " " 
         << playerData.getShotsFired() << " " 
         << playerData.getEnemiesKilled() << " " 
         << playerData.getPlayerDeaths() << " " 
         << playerData.getTimesHit();

    return true;
}

RW::SaveData RWyatt::getSavedPlayerData()
{
    std::ifstream inf {"savedata"};

    if (!inf) {
        std::cerr << "RWyatt::getSavedPlayerData: Save file does not exist\n";
        return RW::SaveData(0, 0, 0, 0, 0);
    }
    
    int score, shots, enemies, deaths, hits;

    // File format:
    // score shots enemies deaths hits
    inf >> score
        >> shots
        >> enemies
        >> deaths
        >> hits;

    return RW::SaveData(score, shots, enemies, deaths, hits);
}

RW::SaveData& RWyatt::getPlayerData()
{
    return playerData;
}

/**
 * WEAPON METHODS
*/
