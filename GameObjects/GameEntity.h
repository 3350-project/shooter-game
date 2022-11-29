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
    Enemy(int, int);
};

class Player : public GameEntity
{
public:
    std::string name = ""; // multiplayer
    Player(float, float);
    void setRotation(float);
    void moveUp(int);
    void moveDown(int);
    void moveLeft(int);
    void moveRight(int);
    void reset();
};

class Bullet : public GameEntity
{
public:
    Bullet(Player);
    std::chrono::time_point<std::chrono::steady_clock> createdAt;  
};

#endif