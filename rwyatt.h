/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for drawing on screen, weapons management, player data.
*/

#include <string>
#include <sstream>
#include <vector>
#include "GameObjects/GameShared.h"

#ifndef RWYATT_H
#define RWYATT_H

namespace RW {
    class SaveData
    {
    public:
        SaveData() : mScore(0), mShotsFired(0), mEnemiesKilled(0),
            mPlayerDeaths(0), mTimesHit(0) 
        {}
        SaveData(
            int score, int shotsFired, int enemiesKilled, int playerDeaths, int timesHit) : 
            mScore(score), mShotsFired(shotsFired), mEnemiesKilled(enemiesKilled),
            mPlayerDeaths(playerDeaths), mTimesHit(timesHit)
        {}
        void addToScore() { mScore++; };
        void subtractFromScore() { mScore--; };
        int getScore() { return mScore; };
        void addToShotsFired() { mShotsFired++; };
        int getShotsFired() { return mShotsFired; };
        void addToEnemiesKilled() { mEnemiesKilled++; };
        int getEnemiesKilled() { return mEnemiesKilled; };
        void addToPlayerDeaths() { mPlayerDeaths++; };
        int getPlayerDeaths() { return mPlayerDeaths; };
        void addToTimesHit() { mTimesHit++; };
        int getTimesHit() { return mTimesHit; };
        std::string asString() {
            std::stringstream ss;
            ss << "SaveData:" << " "
               << mScore << " "
               << mShotsFired << " "
               << mEnemiesKilled << " "
               << mPlayerDeaths << " "
               << mTimesHit;
            return ss.str();
        }
    private:
        int mScore;
        int mShotsFired;
        int mEnemiesKilled;
        int mPlayerDeaths; // todo
        int mTimesHit; // todo
    };
}

namespace RW {
    class WeaponHandler
    {
    private:
        std::vector<Weapon> mPlayerWeapons;
        Weapon mActiveWeapon;
    public:
        WeaponHandler();
        Weapon& getActiveWeapon();
        void setActiveWeapon(int);
        std::vector<Weapon> getAllPlayerWeapons();
        std::vector<Bullet> fireActiveWeapon(Player p);
    };
}

class RWyatt
{
private:
    static void flipState(bool &state);
    bool saveFileExists();
    RW::SaveData mPlayerData;
    RW::SaveData mPreviousPlayerData;
    bool mPromptSaveScore {false};
public:
    static void pauseScreen(bool &paused);
    static void drawBorder(int xres, int yres);
    static void drawWeaponUI(int, int, RW::WeaponHandler&);
    static bool validEnemySpawn(float, float, float, float);
    void drawScore(int, int);
    bool savePlayerData();
    void getSavedPlayerData();
    RW::SaveData& getPreviousPlayerData();
    RW::SaveData& getPlayerData();
    bool getPromptSaveScore();
    void switchPromptSaveScore();
    void drawPromptSaveScore(int, int);
    RWyatt();
};

#endif
