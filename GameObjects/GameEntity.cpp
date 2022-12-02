/**
 * AUTHOR: Reid Wyatt
 * CLASS: CMPS 3350
 * DATE: FALL 2022
 * TEAM: 3
 * ---
 * Contains methods for setting up GameEntities, which 
 * make up all interactive objects in the game.
*/
#include "GameShared.h"

#define rnd() (((float)rand())/(float)RAND_MAX)
#define random(a) (float)(rand()%a)

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
    color = {1.0f, 1.0f, 1.0f,};
    health = 3;
}

void Player::setRotation(float nr) 
{
    rotation = nr;
}

void Player::moveUp(int moveSpeed)
{
    position.y += moveSpeed;
}

void Player::moveDown(int moveSpeed)
{
    position.y -= moveSpeed;
}

void Player::moveLeft(int moveSpeed)
{
    position.x -= moveSpeed;
}
void Player::moveRight(int moveSpeed)
{
    position.x += moveSpeed;
}

Enemy::Enemy(int resolutionX, int resolutionY)
{
    colisionRadius = rnd() * 80.0 + 40.0;

    rotation = 0.0f;
    color = {0.8, 0.8, 0.7};

    position = {random(resolutionX),
                random(resolutionY),
                0.0f};

    velocity = {(float)(rnd() * 2.0 - 0.5),
                (float)(rnd() * 2.0 - 0.5),
                0.0f};

    health = 1;
}

Bullet::Bullet(Player p)
{
    float rad = ((p.rotation + 90.0f) / 360.0f) * M_PI * 2.0;
    float directionX = cos(rad);
    float directionY = sin(rad);
    position.x = p.position.x + directionX * 20.0f;
    position.y = p.position.y + directionY * 20.0f;
    velocity.x = p.velocity.x + directionX * 6.0f + rnd() * 0.1;
    velocity.y = p.velocity.y + directionY * 6.0f + rnd() * 0.1;
    color = {1.0f, 1.0f, 1.0f};
    health = 1;
    createdAt = std::chrono::steady_clock::now();
}
