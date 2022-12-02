/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * File containing base class for creating unique weapons.
*/
#include <chrono>
#ifndef WEAPON_H
#define WEAPON_H

class Weapon
{
public:
    Weapon() {}
    Weapon(std::string w, int m, float f, float v, int d, float l) :
        mWeaponName {w}, mMagizineSize {m}, mCurrentMagizine {m},
        mFireRate {f}, mBulletVelocity {v}, mWeaponDamage {d},
        mBulletLifetime(l)
    {};
    std::string getWeaponName() { return mWeaponName; }
    int getMagizineSize() { return mMagizineSize; }
    int getCurrentMagizine() { return mCurrentMagizine; }
    int getFireRate() { return mFireRate; }
    int getBulletVelocity() { return mBulletVelocity; }
    int getDamage() { return mWeaponDamage; }
    int getBulletLifetime() { return mBulletLifetime; }
    void decCurrentMagizine() { mCurrentMagizine--; }
    void reloadWeapon() { mCurrentMagizine = mMagizineSize; }
private:
    std::string mWeaponName;
    int mMagizineSize;
    int mCurrentMagizine;
    float mFireRate;
    float mBulletVelocity; 
    int mWeaponDamage;
    int mBulletLifetime;
    std::chrono::time_point<std::chrono::steady_clock> mResetTime {};
};

#endif
