#include "enemy.h"
#include "bullet.h"
#include <algorithm>
#include <iostream>
#include <ostream>

Enemy::Enemy(Vector2 position, Vector2 size, float speed, int enemyType, std::vector<Bullet*>& bullets, float shootDelay, std::vector<Enemy*>&enemies, SoundManager* soundManager)
    : position(position), size(size), speed(speed), direction(1), health(5), isActive(true), enemyType(enemyType), bullets(bullets), shootDelay(shootDelay), enemies(enemies) 
    {
        initShootDelay = static_cast<float>(GetRandomValue(0, 10)) / 10.0f;
        enemies.push_back(this);
        // laser2 = LoadSound("../Sounds/Laser/laser2.wav");
        // hitSound = LoadSound("../Sounds/Hit/hit.wav");
        characterIdle = LoadTexture("/rd/Idle(32x32)3.png");
        characterWalk = LoadTexture("/rd/assets/Sprites/VirtualGuy/Run(32x32).png");
        characterShoot = LoadTexture("/rd/assets/Sprites/VirtualGuy/Shoot(32x32).png");
        testTexture = LoadTexture("/rd/test.bmp");
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
        this->soundManager = soundManager;

        for(int i = 0; i < 11; i++) {
            idleFrames[i] = LoadTexture(TextFormat("/rd/assets/Sprites/VirtualGuy/Idle%d.png", i));
            walkFrames[i] = LoadTexture(TextFormat("/rd/assets/Sprites/VirtualGuy/Walk%d.png", i));
        }

    }

void Enemy::Update() {
    if (isHit) {
        health--;
        // PlaySound(hitSound);
        soundManager->PlayHitSound();
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
        // DrawTextureRec(characterShoot, { frameRec.x, frameRec.y, frameRec.width * direction, frameRec.height }, position, WHITE);
        // DrawTexture(characterShoot, position.x, position.y, WHITE);
        Rectangle sourceRec = { 0, 0, (float)characterShoot.width * direction, (float)characterShoot.height };
        Rectangle destRec = { position.x, position.y, size.x, size.y };
        DrawTexturePro(characterShoot, sourceRec, destRec, {0,0}, 0.0f, WHITE);
    }else{
    if (currentSpeed > 0) {
        // DrawTextureRec(characterWalk, { frameRec.x, frameRec.y, frameRec.width * direction, frameRec.height }, position, WHITE);
        // DrawTexture(walkFrames[currentFrame], position.x, position.y, WHITE);

        Rectangle sourceRec = { 0, 0, (float)walkFrames[currentFrame].width * direction, (float)walkFrames[currentFrame].height };
        Rectangle destRec = { position.x, position.y, size.x, size.y };
        DrawTexturePro(walkFrames[currentFrame], sourceRec, destRec, {0,0}, 0.0f, WHITE);
    } else {

        Rectangle sourceRec = { 0, 0, (float)idleFrames[currentFrame].width * direction, (float)idleFrames[currentFrame].height };
        Rectangle destRec = { position.x, position.y, size.x, size.y };
        DrawTexturePro(idleFrames[currentFrame], sourceRec, destRec, {0,0}, 0.0f, WHITE);
        // DrawTexture(idleFrames[currentFrame], position.x, position.y, WHITE);
        }
    }
}

// void Enemy::Draw() {
//     // Draw background rectangle so we know Enemy is being drawn
//     DrawRectangle(position.x, position.y, 32, 32, BLUE);
    
//     // Check texture status
//     if (characterIdle.id == 0) {
//         // Texture failed to load
//         DrawText("LOAD FAIL", position.x, position.y, 10, RED);
//     } else {
//         // Texture loaded - show its properties
//         DrawText(TextFormat("ID:%d", characterIdle.id), position.x, position.y, 10, GREEN);
//         DrawText(TextFormat("%dx%d", characterIdle.width, characterIdle.height), position.x, position.y + 10, 10, GREEN);
        
//         // Try to draw the texture
//         DrawTexture(characterIdle, position.x, position.y + 20, WHITE);
//     }
// }


// void Enemy::Draw() {
//     // Just draw the whole texture, no sprite sheet clipping
//     // Texture2D testTexture = LoadTexture("/rd/test.bmp");
//     if (testTexture.id == 0) {
//         DrawText("LOAD FAILED", 100, 100, 20, RED);
//     } else {
//         DrawText("LOADED OK!", 100, 100, 20, GREEN);
//         DrawTexture(testTexture, 100, 150, WHITE);
//     }
//     // Draw outline for reference
//     // DrawRectangleLines(position.x, position.y, 32, 32, RED);
// }

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
    // PlaySound(laser2);
    soundManager->PlayLaser2Sound();
}
