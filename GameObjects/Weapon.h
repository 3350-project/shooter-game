/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * File containing base class for creating unique weapons.
*/

#ifndef WEAPON_H
#define WEAPON_H

class Weapon
{
public:
    Weapon(std::string w, int m, float f, float v, int d) :
        mWeaponName {w}, mMagizineSize {m}, mCurrentMagizine {m},
        mFireRate {f}, mBulletVelocity {v}, mWeaponDamage {d} 
    {};
    int getCurrentMagizine() { return mCurrentMagizine; }
    int getMagizineSize() { return mMagizineSize; }
    int getDamage() { return mWeaponDamage; }
    std::string getWeaponName() { return mWeaponName; }
    void fireWeapon();
private:
    std::string mWeaponName;
    int mMagizineSize;
    int mCurrentMagizine;
    float mFireRate;
    float mBulletVelocity; 
    int mWeaponDamage;
    std::chrono::time_point<std::chrono::steady_clock> mResetTime {};
};

#endif
