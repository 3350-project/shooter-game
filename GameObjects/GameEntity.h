/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for setting up GameEntities, which 
 * make up all interactive objects in the game.
*/
#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

class GameEntity
{
public:
    Vector3 position;
    Vector3 velocity;
    Vector3 color;
    double rotation;
    float colisionRadius;
    int health;
    void updatePosition();
    void setHealth(int);
    void hitHealth();
};

enum EnemyType {
    Square,
    Triangle,
    Circle
};

class Enemy : public GameEntity
{
public:
    EnemyType type;
    Enemy(int, int);
};

class Player : public GameEntity
{
public:
    std::string name = ""; // multiplayer
    Player(float, float);
    void setRotation(double);
    void moveUp(int);
    void moveDown(int);
    void moveLeft(int);
    void moveRight(int);
};

class Bullet : public GameEntity
{
public:
    Bullet(Player, Weapon, float);
    std::chrono::time_point<std::chrono::steady_clock> createdAt; 
    int lifetime {}; 
};

#endif
