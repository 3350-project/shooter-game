/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for drawing on screen.
*/
#include <string>
#include <sstream>
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

class RWyatt
{
private:
    static void flipState(bool &state);
    bool saveFileExists();
    RW::SaveData playerData;
    RW::SaveData getSavedPlayerData();
public:
    static void pauseScreen(bool &paused);
    static void drawBorder(int xres, int yres);
    static void drawScore(int, int, int);
    bool savePlayerData();
    RW::SaveData& getPlayerData();
    RWyatt();
};

#endif
