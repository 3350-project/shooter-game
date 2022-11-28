#include "GameShared.h"
#include <iostream>

// random number between 0.0 and 1.0
#define randomRatio() static_cast<float>(Random::get(0, RAND_MAX)) / RAND_MAX

namespace Random
{
    // Random number generator
    std::mt19937 mt { std::random_device{}() };

    int get(int min, int max)
    {
        std::uniform_int_distribution<> range{ min, max };
        return range(mt);
    }
}

void GameEntity::updatePosition()
{
    position.x += velocity.x;
    position.y += velocity.y;
}

void GameEntity::setHealth(int hp)
{
    health = hp;
}

void GameEntity::hitHealth()
{
    health -= 1;
}

Player::Player(float resolutionX, float resolutionY)
{
    position = {resolutionX / 2,
                resolutionY / 2, 
                0.0f};
    color = {0.0f, 0.0f, 0.0f,};
    health = 100.0f;
}

void Player::setRotation(float nr) 
{
    rotation = nr;
}

Enemy::Enemy(float resolutionX, float resolutionY)
{
    type = static_cast<EnemyType>(Random::get(0, 2));
    colisionRadius = randomRatio() * 80.0 + 40.0;
    rotation = 0.0f;
    color = {0.8, 0.8, 0.7};

    position = {Random::get(0, resolutionX),
                Random::get(0, resolutionY),
                0.0f};

    velocity = {randomRatio() * 2.0 - 1.0,
                randomRatio() * 2.0 - 1.0,
                0.0f};
    health = 1.0f;
}

Bullet::Bullet(Player p)
{
    float rad = ((p.rotation + 90.0f) / 360.0f) * M_PI * 2.0;
    float directionX = cos(rad);
    float directionY = sin(rad);
    position.x = p.position.x + directionX * 20.0f;
    position.y = p.position.y + directionY * 20.0f;
    velocity.x = p.velocity.x + directionX * 6.0f + randomRatio() * 0.1;
    velocity.y = p.velocity.y + directionY * 6.0f + randomRatio() * 0.1;
    color = {1.0f, 1.0f, 1.0f};
}