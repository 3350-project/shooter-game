#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

class GameEntity
{
public:
    Vector3 position;
    Vector3 velocity;
    Vector3 color;
    float rotation;
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
    Enemy(float, float);
};

class Player : public GameEntity
{
public:
    std::string name = ""; // multiplayer
    Player(float, float);
    void setRotation(float);
};

class Bullet : public GameEntity
{
public:
    Bullet(Player);
};

#endif