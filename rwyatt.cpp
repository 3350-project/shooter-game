/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for drawing on screen, weapons management, player data.
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
    glVertex2i(0 + w, w);
    glVertex2i(0, yres);
    glVertex2i(0 + w, yres - w);
    glVertex2i(xres, yres);
    glVertex2i(xres - w, yres - w);
    glVertex2i(xres, 0);
    glVertex2i(xres - w, w);
    glVertex2i(0, 0);
    glVertex2i(0 + w, w);
    glEnd();
    glDisable(GL_BLEND);
}

void RWyatt::drawScore(int xres, int yres)
{
    Rect r;
    r.bot = yres - 100;
    r.left = xres / 2.5;
    r.center = 0;

    ggprint16(&r, 20, 0x00ffffff, "Score: %d", getPlayerData().getScore());
}

void RWyatt::drawWeaponUI(int xres, int yres, RW::WeaponHandler& wh)
{
    Rect r;
    r.bot = yres - 100;
    r.left = 100;
    r.center = 0;

    int unselected = 0x00000000;
    int selected = 0x00ffffff;
    int ammoColor = 0x00000000;

    Weapon weapon = wh.getActiveWeapon();

    ggprint16(&r, 20, 0x00ffffff, "Available Weapons:");

    int count = 1;
    for (Weapon w : wh.getAllPlayerWeapons()) {
        if (w.getWeaponName() == weapon.getWeaponName()) {
            ggprint16(&r, 20, selected, "   %d. %s", count, w.getWeaponName().c_str());
        } else {
            ggprint16(&r, 20, unselected, "   %d. %s", count, w.getWeaponName().c_str());
        }
        count++;
    }

    if (weapon.getCurrentMagizine() > 0.5 * weapon.getMagizineSize()) {
        ammoColor = 0x0000ff00;
    } else if (weapon.getCurrentMagizine() <= 0.5 * weapon.getMagizineSize() &&
            weapon.getCurrentMagizine() > 0.3 * weapon.getMagizineSize()) {
        ammoColor = 0x00ffa500;
    } else if (weapon.getCurrentMagizine() > 0) {
        ammoColor = 0x00ff0000;
    } else {
        ammoColor = 0x00808080;
    }

    ggprint16(&r, 20, ammoColor, "Magizine: [%d / %d]", weapon.getCurrentMagizine(),
                                                        weapon.getMagizineSize());

    if (weapon.getCurrentMagizine() == 0) {
        Rect m;
        m.bot = yres - 300;
        m.left = xres / 2 - 30;
        m.center = 0;
        ggprint16(&m, 20, 0x00ffffff, "RELOAD [R]");
    }
}

/**
 * CLASS METHODS
*/
RWyatt::RWyatt()
{
    getSavedPlayerData();
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
    std::ofstream outf{"savedata"};

    if (!outf)
    {
        std::cerr << "RWyatt::savePlayerData\n";
        return false;
    }

    outf << mPlayerData.getScore() << " "
         << mPlayerData.getShotsFired() << " "
         << mPlayerData.getEnemiesKilled() << " "
         << mPlayerData.getPlayerDeaths() << " "
         << mPlayerData.getTimesHit();

    return true;
}

void RWyatt::getSavedPlayerData()
{
    std::ifstream inf{"savedata"};

    if (!inf)
    {
        std::cerr << "RWyatt::getSavedPlayerData: Save file does not exist\n";
        mPreviousPlayerData = RW::SaveData(0, 0, 0, 0, 0);
    }

    int score, shots, enemies, deaths, hits;

    // File format:
    // score shots enemies deaths hits
    inf >> score >> shots >> enemies >> deaths >> hits;

    mPreviousPlayerData = RW::SaveData(score, shots, enemies, deaths, hits);
}

RW::SaveData &RWyatt::getPlayerData()
{
    return mPlayerData;
}

RW::SaveData &RWyatt::getPreviousPlayerData()
{
    return mPreviousPlayerData;
}

bool RWyatt::getPromptSaveScore()
{
    return mPromptSaveScore;
}

void RWyatt::switchPromptSaveScore()
{
    flipState(mPromptSaveScore);
}

void RWyatt::drawPromptSaveScore(int xres, int yres)
{
    Rect r;
    r.bot = yres / 2.5 + 100;
    r.left = xres / 2.5;
    r.center = 0;

    ggprint16(&r, 20, 0x00ffffff, "Current Saved Score:");
    ggprint16(&r, 20, 0x00ffffff, "  Score: %d", mPlayerData.getScore());
    ggprint16(&r, 20, 0x00ffffff, "  Shots Fired: %d", mPlayerData.getShotsFired());
    ggprint16(&r, 20, 0x00ffffff, "  Enemies Killed: %d", mPlayerData.getEnemiesKilled());
    ggprint16(&r, 20, 0x00ffffff, "Would you like to save your score? (Y/N)");
}

/**
 * WEAPON METHODS
*/
RW::WeaponHandler::WeaponHandler()
{
    // Magizine Size, Fire Rate, Velocity, Damage, Lifetime

    // Pistol
    mPlayerWeapons.push_back(
        Weapon("Pistol", 15, 1.5, 5.0, 1, 1));
    // Rifle
    mPlayerWeapons.push_back(
        Weapon("Rifle", 30, 1.0, 10.0, 2, 2));
    // Shotgun
    mPlayerWeapons.push_back(
        Weapon("Shotgun", 8, 1.5, 5.0, 1, 2));
    // Sniper
    mPlayerWeapons.push_back(
        Weapon("Sniper", 5, 2.0, 30.0, 3, 3));
    // Machine Gun
    mPlayerWeapons.push_back(
        Weapon("Machine Gun", 45, 0.7, 4.0, 1, 3.0));
}

Weapon& RW::WeaponHandler::getActiveWeapon()
{
    return mActiveWeapon;
}

std::vector<Weapon> RW::WeaponHandler::getAllPlayerWeapons()
{
    return mPlayerWeapons;
}

void RW::WeaponHandler::setActiveWeapon(int weaponId)
{
    if (weaponId < 0 && weaponId > 5)
    {
        std::cerr << "RW::WeaponHandler::setActiveWeapon: Invalid weapon ID" << std::endl;
        return;
    }
    if (mActiveWeapon.getWeaponName() == mPlayerWeapons[weaponId - 1].getWeaponName())
    {
        std::cerr << "RW::WeaponHandler::setActiveWeapon: Already holding that weapon" << std::endl;
        return;
    }
    mActiveWeapon = mPlayerWeapons[weaponId - 1];
}

// Returns a vector of bullets to be added to active bullets
std::vector<Bullet> RW::WeaponHandler::fireActiveWeapon(Player p)
{
    std::vector<Bullet> bullets;

    if (mActiveWeapon.getCurrentMagizine() == 0)
    {
        std::cout << mActiveWeapon.getWeaponName() << " OUT OF AMMO!" << std::endl;
        return bullets;
    }

    // Bullet location is based on player position
    if (mActiveWeapon.getWeaponName() == "Shotgun")
    {
        // create multiple bullets
        for (int angleOffset = -15; angleOffset < 15; angleOffset += 5)
        {
            bullets.push_back(Bullet(p, mActiveWeapon, angleOffset));
        }
    }
    else
    {
        bullets.push_back(Bullet(p, mActiveWeapon, 0.0f));
    }
    mActiveWeapon.decCurrentMagizine();

    return bullets;
}
