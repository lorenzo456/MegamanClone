#include "enemy.h"
#include "bullet.h"
#include <algorithm>
#include <iostream>
#include <ostream>

Enemy::Enemy(Vector2 position, Vector2 size, float speed, int enemyType, std::vector<Bullet*>& bullets, float shootDelay, std::vector<Enemy*>&enemies)
    : position(position), size(size), speed(speed), direction(1), health(5), isActive(true), enemyType(enemyType), bullets(bullets), shootDelay(shootDelay), enemies(enemies) 
    {
        initShootDelay = static_cast<float>(GetRandomValue(0, 10)) / 10.0f;
        enemies.push_back(this);
        laser2 = LoadSound("Sounds/Laser/laser2.wav");
        hitSound = LoadSound("Sounds/Hit/hit.wav");
        characterIdle = LoadTexture("Sprites/VirtualGuy/Idle(32x32).png");
        characterWalk = LoadTexture("Sprites/VirtualGuy/Run(32x32).png");
        characterShoot = LoadTexture("Sprites/VirtualGuy/Shoot(32x32).png");
        frameWidth = (float)(characterIdle.width/11);   // Sprite one frame rectangle width
        frameHeight = (float)(characterIdle.height/1);           // Sprite one frame rectangle height
        currentFrame = 0;
        currentLine = 0;
        frameCounter = 0;
        frameRec = { 0, 0, frameWidth, frameHeight };
        isShooting = false;
        shootingTimer = 0;
        isHit = false;
        velocityY = 0;
        currentSpeed = 0;
    }

void Enemy::Update() {
    if (isHit) {
        health--;
        PlaySound(hitSound);
        isHit = false;
    }

    if (health <= 0) {
        isActive = false;
    }

    if (position.x < 0) position.x = 0;



    velocityY += gravity;
    position.y += velocityY;

    // Collision detection with screen edges
    if (position.x < 0) position.x = 0;
    if (position.x > GetScreenWidth() - size.x) position.x = GetScreenWidth() - size.x;
    if (position.y > GetScreenHeight() - size.y) {
        position.y = GetScreenHeight() - size.y;
        velocityY = 0.0f;
    }


    if (enemyType == 0)
    {
        shootTimer += GetFrameTime();
        if (shootTimer >= shootDelay + initShootDelay) {
            Shoot();
            shootTimer = 0.0f;
            initShootDelay = 0;
        }
    }

    frameCounter++;
    if (frameCounter > 2)
    {
        currentFrame++;

        if (currentFrame >= 11)
        {
            currentFrame = 0;
            currentLine++;

            if (currentLine >= 1)
            {
                currentLine = 0;
            }
        }

        frameCounter = 0;
    }
    frameRec.x = frameWidth*currentFrame;
    frameRec.y = frameHeight*currentLine;


    if (isShooting) {
        shootingTimer += GetFrameTime();
        if (shootingTimer >= 0.1f) {
            isShooting = false;
            shootingTimer = 0;
        }
    }
    currentSpeed = 0;
}

void Enemy::FollowPlayer(Vector2 playerDirection)
{
    if (playerDirection.x > position.x) {
        currentSpeed += speed * GetFrameTime();
        direction = 1;
        position.x += currentSpeed * direction;
    } else if (playerDirection.x < position.x) {    
        currentSpeed += speed * GetFrameTime();
        direction = -1;
        position.x += currentSpeed * direction;
    }
}

void Enemy::Draw() {
    if(isShooting){
        DrawTextureRec(characterShoot, { frameRec.x, frameRec.y, frameRec.width * direction, frameRec.height }, position, WHITE);
    }else{
    if (currentSpeed > 0) {
        DrawTextureRec(characterWalk, { frameRec.x, frameRec.y, frameRec.width * direction, frameRec.height }, position, WHITE);
    } else {

            DrawTextureRec(characterIdle, { frameRec.x, frameRec.y, frameRec.width * direction, frameRec.height }, position, WHITE);
        }
    }
}

Rectangle Enemy::GetRectangle() {
    return { position.x, position.y, size.x, size.y };
}

void Enemy::Shoot() {
    // Vector2 spawnPoint = { position.x - size.x, position.y + size.y / 2 };
    if (direction == 1){
        bulletSpawnpoint = {position.x + size.x + 10, position.y + size.y / 2};
    }else{
        bulletSpawnpoint = {position.x - size.x + 10, position.y + size.y / 2};
    }
    Bullet* tempBullet = new Bullet(bulletSpawnpoint, {10, 5}, direction, 10.0f);
    tempBullet->isActive = true;
    bullets.push_back(tempBullet);
    PlaySound(laser2);
}
